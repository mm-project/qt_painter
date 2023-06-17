#ifdef DUMMY_RQ

#include "RegionQueryService.hpp"
#include "../design.hpp"

RegionQuery::RegionQuery()
{
    m_ws = std::shared_ptr<Design>(new Design);
}

void RegionQuery::insertObject(IShapePtr shape)
{
    m_ws->addObject(shape);
}

void RegionQuery::removeObject(IShapePtr shape)
{
    m_ws->removeObject(shape);
}

IShapePtr RegionQuery::getShapeUnderPos(const QPoint & point) const
{
    for (auto shape : m_ws->getObjects())
        if (shape->contains(point))
            return shape;

    return nullptr;
}

std::vector<IShapePtr> RegionQuery::getShapesUnderRect(const QRect & box) const
{
    std::vector<IShapePtr> shapes;
    // Iterte over all objects in design and check if current shapes point are inside query box
    // consider it is, if at least 1 point is in that rect
    for (auto shape : m_ws->getObjects())
    {
        bool contains = false;

        for (auto point : shape->getPoints())
            if (box.contains(point))
                contains = true;
                //break here actually

        if (contains)
            shapes.push_back(shape);
    }
    return shapes;
}


void RegionQuery::clear() { m_ws->getObjects().clear(); }
void RegionQuery::shutDown() {}
int RegionQuery::getSize() const { return m_ws->getObjects().size(); }

//void RegionQuery::setWS(ObjectPoolPtr ws) {m_ws = ws;}


#endif // ifdef DUMMY_RQ
