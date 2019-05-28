#include "runtime_environment.hpp"

#include "../gui/controller.hpp"

#include "shapes.hpp"
#include "working_set.hpp"

ObjectSandbox::ObjectSandbox(ObjectPoolSandboxPtr p)
	: m_parent(p)
{
	m_pool = std::shared_ptr<WorkingSet>(new WorkingSet);
}

IObjectPoolPtr ObjectSandbox::getPool() const
{
	return m_pool;
}

void ObjectSandbox::clear()
{
	m_pool->clear();
}

void ObjectSandbox::addPoint(QPoint p)
{
	auto obs = m_pool->getObjects();
	for (auto it : obs)
		it->addPoint(p);
}

void ObjectSandbox::movePoint(QPoint p)
{
	auto obs = m_pool->getObjects();
	for (auto it : obs)
		it->movePoint(p);
}

void ObjectSandbox::addObject(IShape* p)
{
	if (p != nullptr)
		m_pool->addObject(p);
}

void ObjectSandbox::changeBasicProperties(const ShapeProperties& b)
{
	auto obs = m_pool->getObjects();
	for (auto it : obs)
		it->updateProperties(b);
}

ObjectPoolSandbox::ObjectPoolSandbox()
{
	/*controller* c = controller::get_instance();
	ShapeProperties b = c->get_shape_properties();
	runtime_object_type = LINE;
	runtime_line = new line(QLine(QPoint(0,0), QPoint(0,0)), b);
	runtime_rectangle = new rectangle(QRect(0, 0 , 0, 0), b);
	runtime_ellipse = new ellipse(QRect(0, 0 , 0, 0), b);
	runtime_polygon = new polygon(QPolygonF(),b);*/
}

std::vector<ObjectSandboxPtr> ObjectPoolSandbox::getChildren() const
{
	return m_children;
}

void ObjectPoolSandbox::addChildren(ObjectSandboxPtr p)
{
	m_children.push_back(p);
}

/*
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

IShape* runtime_environment::get_runtime_object() const
{
	switch (runtime_object_type) {
			case LINE :
					return runtime_line;
			case RECTANGLE:
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
			case RECTANGLE :
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
			case RECTANGLE :
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
			case RECTANGLE:
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
}*/
