#include "main_window.hpp"

#include "canvas.hpp"
#include "create_shape_gui.hpp"
#include "pen_brush_gui.hpp"
#include "icons.hpp"

#include <QDockWidget>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>

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

	QToolBar* homeBar = new QToolBar(this);
	homeBar->addWidget(m_shapes);

	addToolBar(Qt::TopToolBarArea, homeBar);

	setFixedSize(1100, 700);
	setCentralWidget(m_canvas);

	make_connections();
	init_menu();
	static QIcon main_window_icon(IconDir + "shapes_simple.png");
	setWindowIcon(main_window_icon);
}

void main_window::make_connections()
{
	connect(m_shapes, SIGNAL(shape_changed()), m_canvas, SLOT(current_type_changed()));
}

void main_window::init_menu()
{
	/*QMenu* design_menu = menuBar()->addMenu("Design");
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
	create_menu->addAction(polygon_icon, "Polygon", m_canvas, SLOT(create_polygon()));*/
}
