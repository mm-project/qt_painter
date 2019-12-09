#include "design.hpp"
#include "core.hpp"

IShapePtr Design::addObject(IShapePtr s)
{
	ASSERT_RETURN_VALUE(s != nullptr, nullptr);
	auto obj = std::shared_ptr<IShape>(s->clone());
	ASSERT_RETURN_VALUE(obj != nullptr, nullptr);
	m_is_saved = false;
	return ObjectPoolBase::addObject(obj);
}

std::string Design::getName() const noexcept
{
	return std::move("Design");
}

