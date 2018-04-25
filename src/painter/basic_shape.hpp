#ifndef  BASIC_SHAPE_HPP
#define BASIC_SHAPE_HPP

#include <QColor>
#include <QPoint>

class QPainter;


enum object_type {
        LINE = 0,
        RECT,
        ELLIPSE,
        POLYGON
};

struct basic_properties
{
        basic_properties() : pen_color(Qt::white), brush_color(Qt::black), pen_width(1),
                pen_style(Qt::SolidLine), pen_cap_style(Qt::SquareCap), pen_join_style(Qt::BevelJoin), brush_style(Qt::SolidPattern) {}
        QColor pen_color;
        QColor brush_color;
        int pen_width;
        Qt::PenStyle pen_style;
        Qt::PenCapStyle pen_cap_style;
        Qt::PenJoinStyle pen_join_style;
        Qt::BrushStyle brush_style;
};

class basic_shape
{
public:
        basic_shape() {}
        basic_shape(object_type t) : m_type(t) {}
        basic_shape(object_type t, basic_properties p) : m_type(t), m_properties(p) {}

        virtual ~basic_shape() {}

public:
        virtual void reset() = 0;
        virtual void set_pos1(const QPoint&) = 0;
        virtual void set_pos2(const QPoint&) = 0;
        virtual void change_basic_properties(basic_properties b) {m_properties = b;}
        virtual bool is_draw_mode() {return false;}
public:
        virtual basic_shape* clone() = 0;
        virtual void draw(QPainter*) = 0;
protected:
        object_type m_type;
        basic_properties m_properties;
};

#endif
