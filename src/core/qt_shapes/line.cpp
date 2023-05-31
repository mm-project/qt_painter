#include "line.hpp"

#include <QPainter>
#include <QPen>

#include <iostream>

///////////////////////////////////////////////////////////////////////////////
//
// @Line implementation
//
Line::Line(QLine l, ShapeProperties p) : IShape(LINE, p), m_waitForSecondClick(false)
{
    m_object = l;
}

Line *Line::clone()
{
    return new Line(m_object, m_properties);
}

void Line::draw(QPainter *p)
{
    QPen pen(m_properties.pen_color, m_properties.pen_width, m_properties.pen_style, m_properties.pen_cap_style,
             m_properties.pen_join_style);
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

void Line::addPoint(const QPoint &point)
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

void Line::setP1(const QPoint &p)
{
    m_object.setP1(p);
}

void Line::setP2(const QPoint &p)
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

bool Line::contains( const QPoint& point ) const 
{
    QPoint p1 = getP1();
    QPoint p2 = getP2();
    QRectF bbox(p1, p2);
    if (!bbox.contains(point))
        // vertical and horizontal case
        return point.x() == p1.x() || point.x() == p2.x() || point.y() == p1.y() || point.y() == p2.y();
    float x = (float)(point.x() - p1.x()) / (p2.x() - p1.x());
    float y = (float)(point.y() - p1.y()) / (p2.y() - p1.y());
    float out = x / y * 100000 / 100000;
    std::cout << out << std::endl;
    bool b = out > 0.95 && out < 1.25;
    if (!b)
        // vertical and horizontal case
        return point.x() == p1.x() || point.x() == p2.x() || point.y() == p1.y() || point.y() == p2.y();
    return b;
}

bool Line::intersects( const QRect& oRect ) const 
{ 
    const QPoint p1 = getP1();
    const QPoint p2 = getP2();

    return intersectsLine(oRect.topLeft(), oRect.topRight()) ||
            intersectsLine(oRect.topLeft(), oRect.bottomLeft()) ||
            intersectsLine(oRect.bottomLeft(), oRect.bottomRight()) ||
            intersectsLine(oRect.bottomRight(), oRect.topRight()) || oRect.contains(p1) || oRect.contains(p2);
}

bool Line::intersectsLine( const QPoint& a1, const QPoint& b1) const
{
    const QPoint a0 = getP1();
    const QPoint b0 = getP2();

    float d = (b0.x() - a0.x()) * (b1.y() - a1.y()) - (b0.y() - a0.y()) * (b1.x() - b0.x());

    if (d == 0)
        return false;

    float q = (a0.y() - a1.y()) * (b1.x() - a1.x()) - (a0.x() - a1.x()) * (b1.y() - a1.y());
    float r = q / d;
    q = (a0.y() - a1.y()) * (b0.x() - a0.x()) - (a0.x() - a1.x()) * (b0.y() - a0.y());
    float s = q / d;
    if (r < 0 || r > 1 || s < 0 || s > 1)
        return false;

    return true;
}

QPoint Line::center() const 
{ 
    return m_object.center();
}

bool Line::isDisjointFrom( const QRect& ) const 
{
    return false;
}