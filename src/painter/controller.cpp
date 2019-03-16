#include "controller.hpp"

controller* controller::m_instance = 0;

controller::controller()
{
        type = LINE;
        //basic properties?
}

controller* controller::get_instance()
{
        if (m_instance == 0) {
                m_instance = new controller;
        }
        return m_instance;
}

void controller::change_object_type(ObjectType t)
{
        type = t;
}

void controller::change_pen_width(int t)
{
        properties.pen_width = t;
}

void controller::change_pen_color(QColor c)
{
        properties.pen_color = c;
}

void controller::change_brush_color(QColor c)
{
        properties.brush_color = c;
}

void controller::change_pen_style(Qt::PenStyle s)
{
        properties.pen_style = s;
}

void controller::change_pen_cap_style(Qt::PenCapStyle s)
{
        properties.pen_cap_style = s;
}

void controller::change_pen_join_style(Qt::PenJoinStyle s)
{
        properties.pen_join_style = s;
}

void controller::change_brush_style(Qt::BrushStyle s)
{
        properties.brush_style = s;
}

// getter functions
ObjectType controller::get_object_type() const
{
        return type;
}

int controller::get_pen_width() const
{
        return properties.pen_width;
}

QColor controller::get_pen_color() const
{
        return properties.pen_color;
}

QColor controller::get_brush_color() const
{
        return properties.brush_color;
}

Qt::PenStyle controller::get_pen_style() const
{
        return properties.pen_style;
}

Qt::PenCapStyle controller::get_pen_cap_style() const
{
        return properties.pen_cap_style;
}

Qt::PenJoinStyle controller::get_pen_join_style() const
{
        return properties.pen_join_style;
}

Qt::BrushStyle controller::get_brush_style() const
{
        return properties.brush_style;
}
