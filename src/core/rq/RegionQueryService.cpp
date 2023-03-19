#include "RegionQueryService.hpp"
#include "debug_helper.hpp"
#include "rq_object.hpp"

#ifndef NO_RQ
RegionQuery::RegionQuery()
{
    m_tree = std::shared_ptr<rq::RQtree<IShape>>(new rq::RQtree<IShape>());
}

void RegionQuery::insertObject(IShapePtr object)
{
    DBG_RQ("insert", object);
    rq::RQobjectPtr obj;
    switch (object->getType())
    {
    case LINE:
        obj = std::shared_ptr<rq::IRQobject>(new rq::RQline(object));
        break;
    case RECTANGLE:
        obj = std::shared_ptr<rq::IRQobject>(new rq::RQrect(object));
        break;
    case ELLIPSE:
        obj = std::shared_ptr<rq::IRQobject>(new rq::RQellipse(object));
        break;
    case POLYGON:
        obj = std::shared_ptr<rq::IRQobject>(new rq::RQpolygon(object));
        break;
    }
    m_tree->insert(obj);
}

void RegionQuery::removeObject(IShapePtr object)
{
    DBG_RQ("remove", object);
    rq::RQobjectPtr obj;
    switch (object->getType())
    {
    case LINE:
        obj = std::shared_ptr<rq::IRQobject>(new rq::RQline(object));
        break;
    case RECTANGLE:
        obj = std::shared_ptr<rq::IRQobject>(new rq::RQrect(object));
        break;
    case ELLIPSE:
        obj = std::shared_ptr<rq::IRQobject>(new rq::RQellipse(object));
        break;
    case POLYGON:
        obj = std::shared_ptr<rq::IRQobject>(new rq::RQpolygon(object));
        break;
    }
    m_tree->remove(obj);
}

IShapePtr RegionQuery::getShapeUnderPos(const QPoint &p) const
{
    rq::RQobjectPtr obj = m_tree->getObject(rq::CPoint(p));

    if (obj != nullptr)
        return obj->getObject();

    return nullptr;
}

std::vector<IShapePtr> RegionQuery::getShapesUnderRect(const QRect &oRect) const
{
    std::vector<IShapePtr> shapes;

    std::vector<rq::RQobjectPtr> objects = m_tree->getObjects(oRect);

    for (auto it : objects)
        if (it != nullptr)
            shapes.push_back(it->getObject());

    return shapes;
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

#endif // ifndef NO_RQ
