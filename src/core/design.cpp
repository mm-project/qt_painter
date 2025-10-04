#include "design.hpp"
#include "core.hpp"

IShapePtr Design1::addObject(IShapePtr s)
{
    ASSERT_RETURN_VALUE(s != nullptr, nullptr);
    auto obj = std::shared_ptr<IShape>(s->clone());
    ASSERT_RETURN_VALUE(obj != nullptr, nullptr);
    m_is_saved = false;
    return ObjectPoolBase::addObject(obj);
}

std::string Design1::getName() const noexcept
{
    return std::move("Design");
}
