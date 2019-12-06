#include "iobject_pool.hpp"
#include "core.hpp"

void ObjectPoolBase::clear() noexcept
{
	m_shapes.clear();
}

IShapePtr ObjectPoolBase::addObject(IShapePtr shape) 
{
	ASSERT_RETURN_VALUE(shape != nullptr, nullptr);
	m_shapes.push_back(shape);
	return shape;
}

void ObjectPoolBase::removeObject(IShapePtr shape) noexcept
{
	auto it = std::find(m_shapes.begin(), m_shapes.end(), shape);
	if (it == m_shapes.end())
		return;
	m_shapes.erase(it);
}

std::vector<IShapePtr> ObjectPoolBase::getObjects() const noexcept
{
	return m_shapes;
}
