#include "line.hpp"

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
