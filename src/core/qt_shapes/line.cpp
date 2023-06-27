#include "line.hpp"

#include <QPainter>
#include <QPen>

#include <iostream>
#include <cmath>

///////////////////////////////////////////////////////////////////////////////
//
// @Line implementation
//
Line::Line(QLineF l, ShapeProperties p) : IShape(LINE, p), m_waitForSecondClick(false)
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
    return m_object.toLine().p1();
}

QPoint Line::getP2() const
{
    return m_object.toLine().p2();
}

bool Line::contains( const QPoint& p ) const 
{
    const auto a = m_object.p1();
    const auto b = m_object.p2();
    QRectF bbox(a, b);
    if ( !bbox.contains( p ) )
        return false;

    const float EPSILON = 20; // this is supposed to be small but we don't use floating system
    auto x = (b.y() - a.y()) / (b.x() - a.x());     
    auto y = a.y() - x * a.x();
    if ( fabs( p.y() - (x * p.x() + y ) ) < EPSILON)
    {
        return true;
    }

   return false;
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
    return m_object.toLine().center();
}

bool Line::isDisjointFrom( const QRect& ) const 
{
    return false;
}