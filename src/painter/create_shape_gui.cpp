#include "create_shape_gui.hpp"

#include "controller.hpp"
#include "icons.hpp"

// qt Ribbon 
#include "qribbon.h"
#include "qribbongroup.h"
#include "qribbonwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>

///////////////////////////////////////////////////////////////////////////////
//
// implementation create_shape_gui
//
create_shape_gui::create_shape_gui(QWidget* p)
	: QWidget(p)
{
	QRibbonGroup* ribbonGroup = new QRibbonGroup;
	ribbonGroup->setTitle("Shapes");

	QGroupBox* bShapes = new QGroupBox(this);
	m_layout = new QVBoxLayout;

	static QIcon lineIcon(IconDir + "line.png");
	m_line_button = new QPushButton(this);
	m_line_button->setFixedSize(25, 20);
	m_line_button->setIcon(lineIcon);
	m_line_button->setFlat(true);

	m_rect_button = new QPushButton(this);
	static QIcon rectIcon(IconDir + "rectangle.png");
	m_rect_button->setFixedSize(25, 20);
	m_rect_button->setIcon(rectIcon);
	m_rect_button->setFlat(true);

	m_ellipse_button = new QPushButton(this);
	static QIcon ellipseIcon(IconDir + "ellipse.png");
	m_ellipse_button->setFixedSize(25, 20);
	m_ellipse_button->setIcon(ellipseIcon);
	m_ellipse_button->setFlat(true);

	m_polygon_button = new QPushButton(this);
	static QIcon polygonIcon(IconDir + "polygon.png");
	m_polygon_button->setFixedSize(25, 20);
	m_polygon_button->setIcon(polygonIcon);
	m_polygon_button->setFlat(true);

	QHBoxLayout* s_layout = new QHBoxLayout;
	s_layout->addWidget(m_line_button);
	s_layout->addWidget(m_rect_button);
	s_layout->addWidget(m_ellipse_button);
	s_layout->addWidget(m_polygon_button);
	bShapes->setLayout(s_layout);
	
	ribbonGroup->addButton(bShapes);

	QRibbonWidget* ribbonWidget = new QRibbonWidget;
	ribbonWidget->addGroup(ribbonGroup);

	QRibbon* ribbon = new QRibbon;
	ribbon->addTab(ribbonGroup, "Home");

	//QGroupBox* pen_group = new QGroupBox("Pen", this);
	//QGroupBox* brush_group = new QGroupBox("Brush", this);

	m_layout->addWidget(ribbon);
	//m_layout->addWidget(pen_group);
	//m_layout->addWidget(brush_group);
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
