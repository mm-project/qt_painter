#include "rectangle.hpp"

#include <QPainter>
#include <QPen>

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//
// @Rectangle implementation
//
Rectangle::Rectangle(QRect r, ShapeProperties p) : IShape(RECTANGLE, p), m_waitForSecondClick(false)
{
    m_object = r;
}

Rectangle *Rectangle::clone()
{
    return new Rectangle(m_object, m_properties);
}

void Rectangle::draw(QPainter *p)
{
    QPen pen(m_properties.pen_color, m_properties.pen_width, m_properties.pen_style, m_properties.pen_cap_style,
             m_properties.pen_join_style);
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

void Rectangle::setTopLeft(const QPoint &p)
{
    m_object.setTopLeft(p);
}

void Rectangle::setBottomRight(const QPoint &p)
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

void Rectangle::addPoint(const QPoint &point)
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

QPoint Rectangle::center() const 
{
    return m_object.center();
}

bool Rectangle::isDisjointFrom( const QRect& oRect ) const
{
    return ! oRect.contains( m_object );
}