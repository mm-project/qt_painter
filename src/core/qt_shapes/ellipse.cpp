#include "ellipse.hpp"

#include <QPainter>
#include <QPen>

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//
// @Ellipse implementation
//
Ellipse::Ellipse(QRect r, ShapeProperties b) : IShape(ELLIPSE, b), m_waitForSecondClick(false)
{
    m_object = r;
}

Ellipse *Ellipse::clone()
{
    return new Ellipse(m_object, m_properties);
}

void Ellipse::draw(QPainter *p)
{
    QPen pen(m_properties.pen_color, m_properties.pen_width, m_properties.pen_style, m_properties.pen_cap_style,
             m_properties.pen_join_style);
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

void Ellipse::setTopLeft(const QPoint &p)
{
    m_object.setTopLeft(p);
}

void Ellipse::setBottomRight(const QPoint &p)
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

void Ellipse::addPoint(const QPoint &point)
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
