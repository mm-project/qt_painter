#include "shapes.hpp"

#include <QPen>
#include <QPainter>

#include <iostream>

// ================= line ===============
line::line(QLine l, basic_properties p)
        : basic_shape(LINE, p)
{
        m_object = l;
}

line* line::clone()
{
        return new line(m_object, m_properties); //new Qline(*m_object) ?
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
}

void line::set_pos1(const QPoint& p)
{
        m_object.setP1(p);
}

void line::set_pos2(const QPoint& p)
{
        m_object.setP2(p);
}

void line::mousePressEvent(QMouseEvent*)
{
        std::cout << "miouse press" << std::endl;
}
// ================== rectangle ==================
rectangle::rectangle(QRect r, basic_properties p)
        : basic_shape(RECT, p)
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
}

void rectangle::set_pos1(const QPoint& p)
{
        m_object.setTopLeft(p);
}

void rectangle::set_pos2(const QPoint& p)
{
        m_object.setBottomRight(p);
}

rectangle::~rectangle()
{
}

// ================= ellipse =============
ellipse::ellipse(QRect r, basic_properties b)
        : basic_shape(ELLIPSE, b)
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
}

void ellipse::set_pos1(const QPoint& p)
{
        m_object.setTopLeft(p);
}

void ellipse::set_pos2(const QPoint& p)
{
        m_object.setBottomRight(p);
}

// ================= polygon ==================
polygon::polygon(QPolygonF p, basic_properties b)
        : basic_shape(POLYGON, b)
{
        m_object = p;
}

polygon* polygon::clone()
{
        //return new polygon(m_object, m_properties);
}

void polygon::draw(QPainter* p)
{
        //QPen pen(m_properties.pen_color, m_properties.pen_width,
        //                m_properties.pen_style, m_properties.pen_cap_style, m_properties.pen_join_style);
        //QBrush brush(m_properties.brush_color, m_properties.brush_style);
        //p->setBrush(brush);
        //p->setPen(pen);
        //p->drawPolygon(m_object);
}

void polygon::reset()
{
	//m_object.setRect(0, 0, 0, 0);
}

void polygon::set_pos1(const QPoint& p)
{
	//arachin
	//m_object.
}

void polygon::set_pos2(const QPoint& p)
{
	//arachin
	//get 
	//m_object.
}

