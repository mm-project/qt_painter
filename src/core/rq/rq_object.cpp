#include "rq_object.hpp"

namespace rq
{

CPoint RQrect::at(int point) const
{
    if (point > 2)
        // throw;
        return CPoint();

    if (point == 0)
        return CPoint(static_cast<Rectangle *>(m_object.get())->getTopLeft());
    return CPoint(static_cast<Rectangle *>(m_object.get())->getBottomRight());
}

bool RQrect::contains(const CPoint &point) const
{
    // point is null, fix it
    return static_cast<Rectangle *>(m_object.get())->contains(point.get());
}

bool RQrect::intersects(const QRect &oRect) const
{
    return static_cast<Rectangle *>(m_object.get())->intersects(oRect);
}

IShapePtr RQrect::getObject() const
{
    return m_object;
}

CPoint RQellipse::at(int point) const
{
    if (point > 2)
        // throw;
        return CPoint();

    if (point == 0)
        return CPoint(static_cast<Ellipse *>(m_object.get())->getTopLeft());
    return CPoint(static_cast<Ellipse *>(m_object.get())->getBottomRight());
}

bool RQellipse::contains(const CPoint &point) const
{
    return static_cast<Ellipse *>(m_object.get())->contains(point.get());
}

bool RQellipse::intersects(const QRect &oRect) const
{
    return static_cast<Ellipse *>(m_object.get())->intersects(oRect);
}

IShapePtr RQellipse::getObject() const
{
    return m_object;
}

CPoint RQpolygon::at(int point) const
{
    if (point > 2)
        // throw;
        return CPoint();

    if (point == 0)
        return CPoint(static_cast<Polygon *>(m_object.get())->getTopLeft());
    return CPoint(static_cast<Polygon *>(m_object.get())->getBottomRight());
}

bool RQpolygon::contains(const CPoint &point) const
{
    return static_cast<Polygon *>(m_object.get())->contains(point.get());
}

IShapePtr RQpolygon::getObject() const
{
    return m_object;
}

bool RQpolygon::intersects(const QRect &oRect) const
{
    return static_cast<Polygon *>(m_object.get())->intersects(oRect);
}
} // namespace rq