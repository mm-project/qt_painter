#include "runtime_environment.hpp"

#include "shapes.hpp"
#include "controller.hpp"

runtime_environment::runtime_environment()
{
	controller* c = controller::get_instance();
	ShapeProperties b = c->get_shape_properties();
	runtime_object_type = LINE;
	runtime_line = new line(QLine(QPoint(0,0), QPoint(0,0)), b);
	runtime_rectangle = new rectangle(QRect(0, 0 , 0, 0), b);
	runtime_ellipse = new ellipse(QRect(0, 0 , 0, 0), b);
	runtime_polygon = new polygon(QPolygonF(),b);
}

void runtime_environment::reset()
{
	runtime_line->reset();
	runtime_rectangle->reset();
	runtime_ellipse->reset();
	runtime_polygon->reset();
}

void runtime_environment::change_object_type(ObjectType type)
{
	runtime_object_type = type;
}

IBasicShape* runtime_environment::get_runtime_object() const
{
	switch (runtime_object_type) {
			case LINE :
					return runtime_line;
			case RECT:
					return runtime_rectangle;
			case ELLIPSE:
					return runtime_ellipse;
			case POLYGON:
					return runtime_polygon;
	}
	return runtime_line;
}

void runtime_environment::set_pos1(const QPoint& p)
{
	switch (runtime_object_type) {
			case LINE :
					runtime_line->addPoint(p);
					break;
			case RECT :
					runtime_rectangle->addPoint(p);
					break;
			case ELLIPSE:
					runtime_ellipse->addPoint(p);
					break;
			case POLYGON:
					runtime_polygon->addPoint(p);
					break;
	}
}

void runtime_environment::set_pos2(const QPoint& p)
{
	switch (runtime_object_type) {
			case LINE :
					runtime_line->addPoint(p);
					break;
			case RECT :
					runtime_rectangle->addPoint(p);
					break;
			case ELLIPSE:
					runtime_ellipse->addPoint(p);
					break;
			case POLYGON:
					runtime_polygon->addPoint(p);
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
					runtime_polygon->draw(p);;
	}
}

void runtime_environment::change_basic_properties(ShapeProperties b)
{
	runtime_line->updateProperties(b);
	runtime_rectangle->updateProperties(b);
	runtime_ellipse->updateProperties(b);
	runtime_polygon->updateProperties(b);
}
