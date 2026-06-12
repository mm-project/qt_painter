#include "RegionQueryService.hpp"

#ifndef DUMMY_RQ

#include "debug_helper.hpp"
#include "rq_object.hpp"
#include "quard_tree.hpp"

RegionQuery::RegionQuery()
{
    //m_tree = std::shared_ptr<KDtree<IShapePtr>>(new KDtree<IShapePtr>());
    m_tree = std::shared_ptr<QtShapeQuadtree<IShapePtr>>(new QtShapeQuadtree<IShapePtr>());
}

void RegionQuery::insertObject(IShapePtr object)
{
    DBG_RQ("insert", object);
    m_tree->insert(object);
}

void RegionQuery::removeObject(IShapePtr object)
{
    DBG_RQ("remove", object);
    m_tree->remove( object );
}

std::vector<IShapePtr> RegionQuery::getShapeUnderPos(const QPoint &p) const
{
    return m_tree->query(p);
}

std::vector<IShapePtr> RegionQuery::getShapesUnderRect(const QRect &oRect) const
{
    return m_tree->query(oRect);
}

void RegionQuery::clear()
{
    m_tree->clear();
}

void RegionQuery::shutDown()
{
    clear();
}

int RegionQuery::getSize() const
{
    return m_tree->getSize();
}

void RegionQuery::insertObjects(const std::vector<IShapePtr>& objects)
{
    m_tree->insert(objects);
}

#else 

void RegionQuery::insertObjects(const std::vector<IShapePtr>& objects)
{
    for (auto& object : objects) {
        m_ws->addObject(object);
    }
}

#endif
