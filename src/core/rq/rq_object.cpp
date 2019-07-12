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

bool RQrect::intersects(const QRect& oRect) const
{
	return m_object->intersects(oRect);
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

bool RQellipse::intersects(const QRect& oRect) const
{
	return m_object->intersects(oRect);
}

IShape* RQellipse::getObject() const
{
	return m_object;
}

CPoint RQpolygon::at(int point) const
{
	if (point > 2)
		// throw;
		return CPoint();

	if (point == 0)
		return CPoint(m_object->getTopLeft());
	return CPoint(m_object->getBottomRight());
}

bool RQpolygon::contains(const CPoint& point) const
{
	return m_object->contains(point.get());

}

IShape* RQpolygon::getObject() const
{
	return m_object;
}

bool RQpolygon::intersects(const QRect& oRect) const
{
	return m_object->intersects(oRect);
}
}