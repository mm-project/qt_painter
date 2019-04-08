#include "main_window.hpp"

#include "gui_commands.hpp"
#include "canvas.hpp"
#include "create_shape_gui.hpp"
#include "pen_brush_gui.hpp"
#include "icons.hpp"
#include "command_manager.hpp"

#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QEvent>
#include <QCoreApplication>
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QLayout>

#include <cassert>

//FIXME todo enhanced~! (from QApplication)
bool main_window::eventFilter(QObject *obj, QEvent *event)
{
        if (qobject_cast<QRadioButton*>(obj) ) {
            if (event->type() == QEvent::MouseButtonPress ) {
                dicmdguiSelectRadioButton(obj->objectName().toStdString()).log();
            }
        }
        
        if ( QComboBox* cmb = qobject_cast<QComboBox*>(obj) ) {
            if (event->type() == QEvent::MouseButtonRelease ) {
                QString s(cmb->currentText());
                s.replace(" ","/");
                dicmdguiSelectComboValue(obj->objectName().toStdString(),s.toStdString()).log();
            }
        }
        
        
return QMainWindow::eventFilter(obj, event);
}

///////////////////////////////////////////////////////////////////////////////
//
// implemenation of class mainWindow 
//
main_window::main_window(QWidget* p)
        : QMainWindow(p)
{
	m_canvas = new canvas;
	m_shapes = new create_shape_gui(this);
	m_pen_brush =  new pen_brush_gui(this);

	QDockWidget* w = new QDockWidget(this);
	w->setWidget(m_shapes);
	w->setFeatures(QDockWidget::NoDockWidgetFeatures);
	w->setTitleBarWidget(new QWidget());
	addDockWidget(Qt::TopDockWidgetArea, w);

	setFixedSize(1100, 700);
	setCentralWidget(m_canvas);

	make_connections();
	init_menu();
	static QIcon main_window_icon(IconDir + "shapes_simple.png");
	setWindowIcon(main_window_icon);
	QCoreApplication::instance()->installEventFilter(this);
	command_manager::get_instance()->set_main_widget(this);
}

void main_window::make_connections()
{
	//connect(m_pen_brush, SIGNAL(something_changed()), m_canvas, SLOT(on_update()));
	connect(m_shapes, SIGNAL(something_changed()), m_canvas, SLOT(on_update()));
	connect(m_shapes, SIGNAL(createLine()), m_canvas, SLOT(invoke_create_line()));
	connect(m_shapes, SIGNAL(createRect()), m_canvas, SLOT(invoke_create_rect()));
	connect(m_shapes, SIGNAL(createEllipse()), m_canvas, SLOT(invoke_create_ellipse()));
	connect(m_shapes, SIGNAL(createPolygon()), m_canvas, SLOT(invoke_create_polygon()));
}

void main_window::init_menu()
{
	QMenu* design_menu = menuBar()->addMenu("Design");
	design_menu->addAction("New", m_canvas, SLOT(reset()));
	design_menu->addAction("Close" , this, SLOT(close()), tr("Ctrl+C"));
	/*
	QMenu* create_menu = menuBar()->addMenu("Create");
	static QIcon line_icon("icons/line.png");
	create_menu->addAction(line_icon, "Line", m_canvas, SLOT(invoke_create_line()), tr("Ctrl+L"));
	static QIcon rect_icon("icons/rectangle.png");
	create_menu->addAction(rect_icon, "Rectangle", m_canvas, SLOT(invoke_create_rect()), tr("Ctrl+R"));
	static QIcon ellipse_icon("icons/ellipse.png");
	create_menu->addAction(ellipse_icon, "Ellipse", m_canvas, SLOT(invoke_create_ellipse()), tr("Ctrl+E"));
	static QIcon polygon_icon("icons/polygon.png");
	create_menu->addAction(polygon_icon, "Polygon", m_canvas, SLOT(invoke_create_polygon()));*/
}
