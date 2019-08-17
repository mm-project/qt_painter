#include "main_window.hpp"

#include "canvas.hpp"
#include "create_shape_gui.hpp"
#include "pen_brush_gui.hpp"
#include "icons.hpp"
#include "statusbar_manager.hpp"
#include "console.hpp"

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
#include <iostream>

//FIXME todo enhanced~! (from QApplication)
bool main_window::eventFilter(QObject *obj, QEvent *event)
{
        //std::cout << "------("<<obj->objectName().toStdString() << ") " << event->type() << std::endl;
        
        if (qobject_cast<QRadioButton*>(obj) ) {
            if (event->type() == QEvent::MouseButtonPress ) {
                dicmdguiSelectRadioButton(obj->objectName().toStdString()).log();
            }
        }

        if ( QAbstractButton* btn = qobject_cast<QAbstractButton*>(obj) ) {
            if (event->type() == QEvent::MouseButtonPress ) {
                QString s(btn->text());
                //s.replace(" ","/");
                //dicmdguiSelectComboValue(obj->objectName().toStdString(),s.toStdString()).log();
                //std::cout << "("<<s.toStdString() << ") (" << obj->objectName().toStdString() <<")" << std::endl;
            }
        }
        //qcomboboxlist
        /*
        if ( QComboBoxListView* cmb = qobject_cast<QComboBox*>(obj) ) {
            //if (event->type() == QEvent::MouseButtonPress ) {
                QString s(cmb->currentText());
                s.replace(" ","/");
                //dicmdguiSelectComboValue(obj->objectName().toStdString(),s.toStdString()).log();
                std::cout << "("<<s.toStdString() << ") (" << obj->objectName().toStdString() <<")" << event->type() << std::endl;
            //}
        }*/
        /*
        if ( QComboBox* cmb = qobject_cast<QComboBox*>(obj) ) {
            //if (event->type() == QEvent::MouseButtonPress ) {
                QString s(cmb->currentText());
                s.replace(" ","/");
                //dicmdguiSelectComboValue(obj->objectName().toStdString(),s.toStdString()).log();
                std::cout << "("<<s.toStdString() << ") (" << obj->objectName().toStdString() <<")" << event->type() << std::endl;
            //}
        }*/
        
        
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
	m_console = new Console(this);
	m_console->setFixedHeight(145);

	QDockWidget* w = new QDockWidget(this);
	w->setWidget(m_shapes);
	w->setFeatures(QDockWidget::NoDockWidgetFeatures);
	w->setTitleBarWidget(new QWidget(this));
	w->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	addDockWidget(Qt::TopDockWidgetArea, w);

	QDockWidget* console_widget = new QDockWidget("Console", this);
	console_widget->setWidget(m_console);
	addDockWidget(Qt::BottomDockWidgetArea, console_widget);

	resize(1200, 800);
	setCentralWidget(m_canvas);

	make_connections();
	static QIcon main_window_icon(getIconDir() + "shapes_simple.png");
	setWindowIcon(main_window_icon);
	QCoreApplication::instance()->installEventFilter(this);
	command_manager::get_instance()->set_main_widget(this);
	StatusBarManager& sBar = StatusBarManager::getInstance();
	sBar.setStatusBar(statusBar());

}

void main_window::make_connections()
{
	connect(m_shapes, SIGNAL(something_changed()), m_canvas, SLOT(on_update()));
	connect(m_shapes, SIGNAL(createLine()), m_canvas, SLOT(invoke_create_line()));
	connect(m_shapes, SIGNAL(createRect()), m_canvas, SLOT(invoke_create_rect()));
	connect(m_shapes, SIGNAL(createEllipse()), m_canvas, SLOT(invoke_create_ellipse()));
	connect(m_shapes, SIGNAL(createPolygon()), m_canvas, SLOT(invoke_create_polygon()));
	connect(m_shapes, SIGNAL(reset()), m_canvas, SLOT(reset()));
	connect(m_shapes, SIGNAL(close()), this, SLOT(close()));
	connect(m_shapes, SIGNAL(selectByRegion()), m_canvas, SLOT(invoke_select_by_region()));
	connect(m_shapes, SIGNAL(selectByPoint()), m_canvas, SLOT(invoke_select_by_point()));
	connect(m_shapes, SIGNAL(save()), m_canvas, SLOT(invoke_save()));
	connect(m_shapes, SIGNAL(load()), m_canvas, SLOT(invoke_load()));
}

main_window::~main_window()
{
	StatusBarManager& sBar = StatusBarManager::getInstance();
	sBar.removeStatusBar();
}
