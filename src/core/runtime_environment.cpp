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
