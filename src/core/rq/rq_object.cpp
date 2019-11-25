#include "rq_object.hpp"

namespace rq
{

CPoint RQrect::at(int point) const
{
	if (point > 2)
		// throw;
		return CPoint();

	if (point == 0)
		return CPoint(m_object->getTopLeft());
	return CPoint(m_object->getBottomRight());
}

bool RQrect::contains(const CPoint& point) const
{
	// point is null, fix it
	return m_object->contains(point.get());

}

IShape* RQrect::getObject() const
{
	return m_object;
}

CPoint RQellipse::at(int point) const
{
	if (point > 2)
		// throw;
		return CPoint();

	if (point == 0)
		return CPoint(m_object->getTopLeft());
	return CPoint(m_object->getBottomRight());
}

bool RQellipse::contains(const CPoint& point) const
{
	return m_object->contains(point.get());

}

IShape* RQellipse::getObject() const
{
	return m_object;
}

}