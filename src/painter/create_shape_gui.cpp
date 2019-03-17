#include "create_shape_gui.hpp"

#include "controller.hpp"

#include <QVBoxLayout>
#include <QRadioButton>

///////////////////////////////////////////////////////////////////////////////
//
// implementation create_shape_gui
//
create_shape_gui::create_shape_gui(QWidget* p)
	: QWidget(p)
{
	m_layout = new QVBoxLayout;

	m_line_button = new QRadioButton("Line", this);
	m_rect_button = new QRadioButton("Rectangle", this);
	m_ellipse_button = new QRadioButton("Ellipse", this);
	m_polygon_button = new QRadioButton("Polygon", this);

	//m_layout->addWidget(m_line_button);
	//m_layout->addWidget(m_rect_button);
	//m_layout->addWidget(m_ellipse_button);
	//m_layout->addWidget(m_polygon_button);
	m_layout->addStretch();

	setLayout(m_layout);

	make_connections();
	m_line_button->click();
}

void create_shape_gui::make_connections()
{
	connect(m_line_button, SIGNAL(pressed()), this, SLOT(change_to_line()));
	connect(m_rect_button, SIGNAL(pressed()), this, SLOT(change_to_rect()));
	connect(m_ellipse_button, SIGNAL(pressed()), this, SLOT(change_to_ellipse()));
	connect(m_polygon_button, SIGNAL(pressed()), this, SLOT(change_to_polygon()));
}

void create_shape_gui::change_to_line()
{
	controller* c = controller::get_instance();
	//c->change_object_type(LINE);
	emit shape_changed();
}

void create_shape_gui::change_to_rect()
{
	controller* c = controller::get_instance();
	//c->change_object_type(RECT);
	emit shape_changed();
}

void create_shape_gui::change_to_ellipse()
{
	controller* c = controller::get_instance();
	//c->change_object_type(ELLIPSE);
	emit shape_changed();
}

void create_shape_gui::change_to_polygon()
{
	controller* c = controller::get_instance();
	//c->change_object_type(POLYGON);
	emit shape_changed();
}
