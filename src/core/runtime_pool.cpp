#include "runtime_pool.hpp"
#include "core.hpp"

#include "../gui/controller.hpp"

#include "shapes.hpp"
#include "design.hpp"

RuntimePool::RuntimePool(RuntimePoolManagerPtr p)
	: m_parent(p)
{
}

IShapePtr RuntimePool::addObject(IShapePtr s)
{
	//	Some commands consider object is cloned
	ASSERT_RETURN_VALUE(s != nullptr, nullptr);
	auto obj = std::shared_ptr<IShape>(s->clone());
	ASSERT_RETURN_VALUE(obj != nullptr, nullptr);
	return ObjectPoolBase::addObject(obj);
}

void RuntimePool::addPoint(QPoint p)
{
	for (auto it : m_shapes)
		it->addPoint(p);
}

void RuntimePool::movePoint(QPoint p)
{
	for (auto it : m_shapes)
		it->movePoint(p);
}

void RuntimePool::changeBasicProperties(const ShapeProperties& b)
{
	for (auto it : m_shapes)
		it->updateProperties(b);
}

std::string RuntimePool::getName() const noexcept
{
	return m_name;
	//std::move("Runtime");
}

void RuntimePool::setName(const std::string& s) 
{
    m_name = s;
}


/************** RuntimePoolManager *************/
//RuntimePoolManager::RuntimePoolManager()
//{
//}

void RuntimePoolManager::addChild(RuntimePoolPtr p, const std::string& name)
{
	ASSERT_RETURN(p != nullptr);
	m_children[name] = p;
    p->setName(name);
}

RuntimePoolPtr RuntimePoolManager::getChild(const std::string& name) const
{
	// throw erroe if not exists
	auto it = m_children.find(name);
	if (it != m_children.end())
		return (*it).second;
	return nullptr;
}

std::map<std::string, RuntimePoolPtr> RuntimePoolManager::getChildren() const noexcept
{
	return m_children;
}
