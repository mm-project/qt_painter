#ifdef DUMMY_RQ

#include "RegionQueryService.hpp"
#include "../design.hpp"

#include <iostream>

RegionQuery::RegionQuery()
{
    m_ws = std::shared_ptr<ObjectPoolBase>(new ObjectPoolBase);
}

void RegionQuery::insertObject(IShapePtr shape)
{
    m_ws->addObject(shape);
}

void RegionQuery::removeObject(IShapePtr shape)
{
    m_ws->removeObject(shape);
}

std::vector<IShapePtr> RegionQuery::getShapeUnderPos(const QPoint & point) const
{
    std::vector<IShapePtr> shapes;

    for (auto shape : m_ws->getObjects())
        if (shape->contains(point))
            shapes.push_back(shape);

    return shapes;
}

std::vector<IShapePtr> RegionQuery::getShapesUnderRect(const QRect & query_rect) const
{
    std::vector<IShapePtr> shapes;

    for (auto shape : m_ws->getObjects())
        if (shape->intersects(query_rect))
            shapes.push_back(shape);

    return shapes;
}


void RegionQuery::clear() {
    m_ws->clear();
    m_ws->getObjects().clear();
}

void RegionQuery::shutDown() {

}

int RegionQuery::getSize() const {
    return m_ws->getObjects().size();
}


#endif // ifdef DUMMY_RQ
