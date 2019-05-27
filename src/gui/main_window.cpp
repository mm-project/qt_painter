#include "main_window.hpp"

#include "canvas.hpp"
#include "create_shape_gui.hpp"
#include "pen_brush_gui.hpp"
#include "icons.hpp"

#include "../commands/command_manager.hpp"
#include "../commands/gui_commands.hpp"

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
	m_canvas = new canvas(this);
	m_shapes = new create_shape_gui(this);

	QDockWidget* w = new QDockWidget(this);
	w->setWidget(m_shapes);
	w->setFeatures(QDockWidget::NoDockWidgetFeatures);
	w->setTitleBarWidget(new QWidget(this));
	addDockWidget(Qt::TopDockWidgetArea, w);

	setFixedSize(1100, 700);
	setCentralWidget(m_canvas);

	make_connections();
	static QIcon main_window_icon(getIconDir() + "shapes_simple.png");
	setWindowIcon(main_window_icon);
	QCoreApplication::instance()->installEventFilter(this);
	command_manager::get_instance()->set_main_widget(this);
}

void main_window::make_connections()
{
	connect(m_shapes, SIGNAL(something_changed()), m_canvas, SLOT(on_update()));
	connect(m_shapes, SIGNAL(createLine()), m_canvas, SLOT(invoke_create_line()));
	connect(m_shapes, SIGNAL(createRect()), m_canvas, SLOT(invoke_create_rect()));
	connect(m_shapes, SIGNAL(createEllipse()), m_canvas, SLOT(invoke_create_ellipse()));
	connect(m_shapes, SIGNAL(createPolygon()), m_canvas, SLOT(invoke_create_polygon()));
	connect(m_shapes, SIGNAL(reset()), m_canvas, SLOT(reset()));
	connect(m_shapes, SIGNAL(close()), m_canvas, SLOT(close()));
	connect(m_shapes, SIGNAL(selectByRegion()), m_canvas, SLOT(invoke_select_by_region()));
}
