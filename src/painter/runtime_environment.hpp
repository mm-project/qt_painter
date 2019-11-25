#ifndef RUNTIME_ENVIRONMENT_HPP
#define RUNTIME_ENVIRONMENT_HPP

#include "basic_shape.hpp"

class line;
class rectangle;
class ellipse;
class polygon;

class runtime_environment
{
public:
        runtime_environment();

        void reset();

        void change_object_type(ObjectType);

        IBasicShape* get_runtime_object() const;

        void set_pos1(const QPoint&);
        void set_pos2(const QPoint&);

        void draw_runtime(QPainter* p);

        void change_basic_properties(ShapeProperties);
private:
        ObjectType runtime_object_type;

        line* runtime_line;
        rectangle* runtime_rectangle;
        ellipse* runtime_ellipse;
        polygon* runtime_polygon;
};

#endif
