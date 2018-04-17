#include "runtime_environment.hpp"

#include "shapes.hpp"
#include "controller.hpp"

runtime_environment::runtime_environment()
{
        controller* c = controller::get_instance();
        basic_properties b = c->get_basic_properties();
        runtime_object_type = LINE;
        runtime_line = new line(QLine(QPoint(0,0), QPoint(0,0)), b);
        runtime_rectangle = new rectangle(QRect(0, 0 , 0, 0), b);
        runtime_ellipse = new ellipse(QRect(0, 0 , 0, 0), b);
}

void runtime_environment::reset()
{
        runtime_line->reset();
        runtime_rectangle->reset();
        runtime_ellipse->reset();
}

void runtime_environment::change_object_type(object_type type)
{
        runtime_object_type = type;
}

basic_shape* runtime_environment::get_runtime_object() const
{
        switch (runtime_object_type) {
                case LINE :
                        return runtime_line;
                case RECT:
                        return runtime_rectangle;
                case ELLIPSE:
                        return runtime_ellipse;
                case POLYGON:
                        break;
        }
        return runtime_line;
}

void runtime_environment::set_pos1(const QPoint& p)
{
        switch (runtime_object_type) {
                case LINE :
                        runtime_line->set_pos1(p);
                        break;
                case RECT :
                        runtime_rectangle->set_pos1(p);
                        break;
                case ELLIPSE:
                        runtime_ellipse->set_pos1(p);
                        break;
                case POLYGON:
                        break;
        }
}

void runtime_environment::set_pos2(const QPoint& p)
{
        switch (runtime_object_type) {
                case LINE :
                        runtime_line->set_pos2(p);
                        break;
                case RECT :
                        runtime_rectangle->set_pos2(p);
                        break;
                case ELLIPSE:
                        runtime_ellipse->set_pos2(p);
                        break;
                case POLYGON:
                        break;
        }
}

void runtime_environment::draw_runtime(QPainter* p)
{
        switch (runtime_object_type) {
                case LINE:
                        runtime_line->draw(p);
                        break;
                case RECT:
                        runtime_rectangle->draw(p);
                        break;
                case ELLIPSE:
                        runtime_ellipse->draw(p);
                        break;
                case POLYGON:
                        break;
        }
}

void runtime_environment::change_basic_properties(basic_properties b)
{
        runtime_line->change_basic_properties(b);
        runtime_rectangle->change_basic_properties(b);
        runtime_ellipse->change_basic_properties(b);
}
