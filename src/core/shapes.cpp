#include "shapes.hpp"

#include <QPen>
#include <QPainter>

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//
// @Line implementation 
//
Line::Line(QLine l, ShapeProperties p)
	: IShape(LINE, p),
	m_waitForSecondClick(false)
{
	m_object = l;
}

Line* Line::clone()
{
	return new Line(m_object, m_properties);
}

void Line::draw(QPainter* p)
{
	QPen pen(m_properties.pen_color, m_properties.pen_width, m_properties.pen_style,
					m_properties.pen_cap_style, m_properties.pen_join_style);
	QBrush brush(m_properties.brush_color, m_properties.brush_style);
	p->setBrush(brush);
	p->setPen(pen);
	p->drawLine(m_object);
}

void Line::reset()
{
	m_object.setLine(0, 0, 0, 0);
	m_waitForSecondClick = false;
}

void Line::addPoint(const QPoint& point)
{
	if (m_waitForSecondClick)
	{
		m_object.setP2(point);
	}
	else
	{
		m_object.setP1(point);
		m_waitForSecondClick = true;
	}
}

void Line::setP1(const QPoint& p)
{
	m_object.setP1(p);
}

void Line::setP2(const QPoint& p)
{
	m_object.setP2(p);
}

QPoint Line::getP1() const
{
	return m_object.p1();
}

QPoint Line::getP2() const
{
	return m_object.p2();
}

///////////////////////////////////////////////////////////////////////////////
//
// @Rectangle implementation 
//
Rectangle::Rectangle(QRect r, ShapeProperties p)
	: IShape(RECTANGLE, p),
	m_waitForSecondClick(false)
{
	m_object = r;
}

Rectangle* Rectangle::clone()
{
	return new Rectangle(m_object, m_properties);
}

void Rectangle::draw(QPainter* p)
{
	QPen pen(m_properties.pen_color, m_properties.pen_width,
					m_properties.pen_style, m_properties.pen_cap_style, m_properties.pen_join_style);
	QBrush brush(m_properties.brush_color, m_properties.brush_style);
	p->setBrush(brush);
	p->setPen(pen);
	p->drawRect(m_object);
}

void Rectangle::reset()
{
	m_object.setRect(0, 0, 0, 0);
	m_waitForSecondClick = false;
}

void Rectangle::setTopLeft(const QPoint& p)
{
	m_object.setTopLeft(p);
}

void Rectangle::setBottomRight(const QPoint& p)
{
	m_object.setBottomRight(p);
}

QPoint Rectangle::getTopLeft() const
{
	return m_object.topLeft();
}

QPoint Rectangle::getBottomRight() const
{
	return m_object.bottomRight();
}

void Rectangle::addPoint(const QPoint& point)
{
	if (m_waitForSecondClick)
	{
		m_object.setBottomRight(point);
	}
	else
	{
		m_object.setTopLeft(point);
		m_waitForSecondClick = true;
	}

}

///////////////////////////////////////////////////////////////////////////////
//
// @Ellipse implementation 
//
Ellipse::Ellipse(QRect r, ShapeProperties b)
	: IShape(ELLIPSE, b),
	m_waitForSecondClick(false)
{
	m_object = r;
}

Ellipse* Ellipse::clone()
{
	return new Ellipse(m_object, m_properties);
}

void Ellipse::draw(QPainter* p)
{
	QPen pen(m_properties.pen_color, m_properties.pen_width,
					m_properties.pen_style, m_properties.pen_cap_style, m_properties.pen_join_style);
	QBrush brush(m_properties.brush_color, m_properties.brush_style);
	p->setBrush(brush);
	p->setPen(pen);
	p->drawEllipse(m_object);
}

void Ellipse::reset()
{
	m_object.setRect(0, 0, 0, 0);
	m_waitForSecondClick = false;
}

void Ellipse::setTopLeft(const QPoint& p)
{
	m_object.setTopLeft(p);
}

void Ellipse::setBottomRight(const QPoint& p)
{
	m_object.setBottomRight(p);
}

QPoint Ellipse::getTopLeft() const
{
	return m_object.topLeft();
}

QPoint Ellipse::getBottomRight() const
{
	return m_object.bottomRight();
}

void Ellipse::addPoint(const QPoint& point)
{
	if (m_waitForSecondClick)
	{
		m_object.setBottomRight(point);
	}
	else
	{
		m_object.setTopLeft(point);
		m_waitForSecondClick = true;
	}

}

///////////////////////////////////////////////////////////////////////////////
//
// @Polygon implementation 
//
Polygon::Polygon(QPolygon p, ShapeProperties b)
	: IShape(POLYGON, b)
{
	m_object = p;
}

Polygon* Polygon::clone()
{
	return new Polygon(m_object, m_properties);
}

void Polygon::draw(QPainter* p)
{
	QPen pen(m_properties.pen_color, m_properties.pen_width,
					m_properties.pen_style, m_properties.pen_cap_style, m_properties.pen_join_style);
	QBrush brush(m_properties.brush_color, m_properties.brush_style);
	p->setBrush(brush);
	p->setPen(pen);
	p->drawPolygon(m_object);
}

void Polygon::reset()
{
	QPolygon p;
	m_object.swap(p);
}

void Polygon::addPoint(const QPoint& p)
{
	if (m_first)
		m_object << p;
	m_object << p;
	m_first = false;
}

void Polygon::movePoint(const QPoint& p)
{
	m_object.setPoint(m_object.size() - 1, p);
}

QPoint Polygon::getTopLeft() const
{
	return m_object.boundingRect().topLeft();
}

QPoint Polygon::getBottomRight() const
{
	return m_object.boundingRect().bottomRight();
}
