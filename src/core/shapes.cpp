#include "shapes.hpp"

#include <QPen>
#include <QPainter>

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//
// @line implementation 
//
line::line(QLine l, ShapeProperties p)
	: IShape(LINE, p),
	m_waitForSecondClick(false)
{
	m_object = l;
}

line* line::clone()
{
	return new line(m_object, m_properties);
}

void line::draw(QPainter* p)
{
	QPen pen(m_properties.pen_color, m_properties.pen_width, m_properties.pen_style,
					m_properties.pen_cap_style, m_properties.pen_join_style);
	QBrush brush(m_properties.brush_color, m_properties.brush_style);
	p->setBrush(brush);
	p->setPen(pen);
	p->drawLine(m_object);
}

void line::reset()
{
	m_object.setLine(0, 0, 0, 0);
	m_waitForSecondClick = false;
}

void line::addPoint(const QPoint& point)
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

void line::setP1(const QPoint& p)
{
	m_object.setP1(p);
}

void line::setP2(const QPoint& p)
{
	m_object.setP2(p);
}

///////////////////////////////////////////////////////////////////////////////
//
// @rectangle implementation 
//
rectangle::rectangle(QRect r, ShapeProperties p)
	: IShape(RECTANGLE, p),
	m_waitForSecondClick(false)
{
	m_object = r;
}

rectangle* rectangle::clone()
{
	return new rectangle(m_object, m_properties);
}

void rectangle::draw(QPainter* p)
{
	QPen pen(m_properties.pen_color, m_properties.pen_width,
					m_properties.pen_style, m_properties.pen_cap_style, m_properties.pen_join_style);
	QBrush brush(m_properties.brush_color, m_properties.brush_style);
	p->setBrush(brush);
	p->setPen(pen);
	p->drawRect(m_object);
}

void rectangle::reset()
{
	m_object.setRect(0, 0, 0, 0);
	m_waitForSecondClick = false;
}

void rectangle::setTopLeft(const QPoint& p)
{
	m_object.setTopLeft(p);
}

void rectangle::setBottomRight(const QPoint& p)
{
	m_object.setBottomRight(p);
}

QPoint rectangle::getTopLeft() const
{
	return m_object.topLeft();
}

QPoint rectangle::getBottomRight() const
{
	return m_object.bottomRight();
}

void rectangle::addPoint(const QPoint& point)
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
// @ellipse implementation 
//
ellipse::ellipse(QRect r, ShapeProperties b)
	: IShape(ELLIPSE, b),
	m_waitForSecondClick(false)
{
	m_object = r;
}

ellipse* ellipse::clone()
{
	return new ellipse(m_object, m_properties);
}

void ellipse::draw(QPainter* p)
{
	QPen pen(m_properties.pen_color, m_properties.pen_width,
					m_properties.pen_style, m_properties.pen_cap_style, m_properties.pen_join_style);
	QBrush brush(m_properties.brush_color, m_properties.brush_style);
	p->setBrush(brush);
	p->setPen(pen);
	p->drawEllipse(m_object);
}

void ellipse::reset()
{
	m_object.setRect(0, 0, 0, 0);
	m_waitForSecondClick = false;
}

void ellipse::setTopLeft(const QPoint& p)
{
	m_object.setTopLeft(p);
}

void ellipse::setBottomRight(const QPoint& p)
{
	m_object.setBottomRight(p);
}

QPoint ellipse::getTopLeft() const
{
	return m_object.topLeft();
}

QPoint ellipse::getBottomRight() const
{
	return m_object.bottomRight();
}

void ellipse::addPoint(const QPoint& point)
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
// @polygon implementation 
//
polygon::polygon(QPolygon p, ShapeProperties b)
	: IShape(POLYGON, b)
{
	m_object = p;
}

polygon* polygon::clone()
{
	return new polygon(m_object, m_properties);
}

void polygon::draw(QPainter* p)
{
	QPen pen(m_properties.pen_color, m_properties.pen_width,
					m_properties.pen_style, m_properties.pen_cap_style, m_properties.pen_join_style);
	QBrush brush(m_properties.brush_color, m_properties.brush_style);
	p->setBrush(brush);
	p->setPen(pen);
	p->drawPolygon(m_object);
}

void polygon::reset()
{
	QPolygon p;
	m_object.swap(p);
}

void polygon::addPoint(const QPoint& p)
{
	if (m_first)
		m_object << p;
	m_object << p;
	m_first = false;
}

void polygon::movePoint(const QPoint& p)
{
	m_object.setPoint(m_object.size() - 1, p);
}

QPoint polygon::getTopLeft() const
{
	return m_object.boundingRect().topLeft();
}

QPoint polygon::getBottomRight() const
{
	return m_object.boundingRect().bottomRight();
}
