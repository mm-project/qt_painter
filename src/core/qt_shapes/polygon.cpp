#include "polygon.hpp"

#include <QPainter>
#include <QPen>

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//
// @polygon implementation
//
Polygon::Polygon(QPolygon p, ShapeProperties b) : IShape(POLYGON, b)
{
    m_object = p;
}

Polygon *Polygon::clone()
{
    return new Polygon(m_object, m_properties);
}

void Polygon::draw(QPainter *p)
{
    QPen pen(m_properties.pen_color, m_properties.pen_width, m_properties.pen_style, m_properties.pen_cap_style,
             m_properties.pen_join_style);
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

void Polygon::addPoint(const QPoint &p)
{
    if (m_first)
        m_object << p;
    m_object << p;
    m_first = false;
}

void Polygon::movePoint(const QPoint &p)
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

QPoint Polygon::center() const 
{
    return m_object.boundingRect().bottomRight();
    // What is center for the polygon?
    //return m_object.boundingRect().center(); 
}

bool Polygon::isDisjointFrom( const QRect& ) const
{
    return false;
}

bool Polygon::contains(const QPoint &point) const
{
    return m_object.containsPoint( point, Qt::OddEvenFill );
}