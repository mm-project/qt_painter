#include "main_window.hpp"

#include "canvas.hpp"
#include "create_shape_gui.hpp"
#include "pen_brush_gui.hpp"

#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QEvent>
#include <QCoreApplication>
#include <QPushButton>
#include <QRadioButton>

#include <cassert>


bool main_window::eventFilter(QObject *obj, QEvent *event)
{
    if (qobject_cast<QRadioButton*>(obj) ) {
        if (event->type() == QEvent::MouseButtonPress ) {
            std::cout << obj->objectName().toStdString() << std::endl;
            assert(0);
            //QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            //qDebug() << "Ate key press" << keyEvent->key();
            return true;
        } else {
            return false;
        }
    } else {
        // pass the event on to the parent class
        return QMainWindow::eventFilter(obj, event);
    }
}

main_window::main_window(QWidget* p)
        : QMainWindow(p)
{
        m_canvas = new canvas;
        m_shapes = new create_shape_gui(this);
        m_pen_brush =  new pen_brush_gui(this);

        QDockWidget* shapes = new QDockWidget("Shapes", this);
        shapes->setWidget(m_shapes);
        shapes->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        QDockWidget* pen_brush = new QDockWidget("Pen and Brush", this);
        pen_brush->setWidget(m_pen_brush);
        pen_brush->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

        addDockWidget(Qt::RightDockWidgetArea, shapes);
        addDockWidget(Qt::RightDockWidgetArea, pen_brush);

        setFixedSize(1100, 700);
        setCentralWidget(m_canvas);

        make_connections();
        init_menu();
        static QIcon main_window_icon("icons/shapes_simple.png");
        setWindowIcon(main_window_icon);

        setDockOptions(QMainWindow::AllowTabbedDocks);
        QCoreApplication::instance()->installEventFilter(this);
}

void main_window::make_connections()
{
        //connect(m_shapes, SIGNAL(shape_changed()), m_canvas, SLOT(current_type_changed()));
        connect(m_pen_brush, SIGNAL(something_changed()), m_canvas, SLOT(on_update()));
}

void main_window::init_menu()
{
        QMenu* design_menu = menuBar()->addMenu("Design");
        design_menu->addAction("New", m_canvas, SLOT(reset()));
        design_menu->addAction("Close" , this, SLOT(close()), tr("Ctrl+C"));

        QMenu* create_menu = menuBar()->addMenu("Create");
        static QIcon line_icon("icons/line.png");
        create_menu->addAction(line_icon, "Line", m_canvas, SLOT(create_line()), tr("Ctrl+L"));
        static QIcon rect_icon("icons/rectangle.png");
        create_menu->addAction(rect_icon, "Rectangle", m_canvas, SLOT(create_rect()), tr("Ctrl+R"));
        static QIcon ellipse_icon("icons/ellipse.png");
        create_menu->addAction(ellipse_icon, "Ellipse", m_canvas, SLOT(create_ellipse()), tr("Ctrl+E"));
        static QIcon polygon_icon("icons/polygon.png");
        create_menu->addAction(polygon_icon, "Polygon", m_canvas, SLOT(create_polygon()));
}
