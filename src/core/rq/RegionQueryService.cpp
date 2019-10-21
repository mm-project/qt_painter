#include "RegionQueryService.hpp"

#include "rq_object.hpp"

RegionQuery::RegionQuery()
{
	m_tree = std::shared_ptr<rq::RQtree<IShape>> (new rq::RQtree<IShape>());
}

void RegionQuery::insertObject(IShape* object)
{
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
 
void RegionQuery::removeObject(IShape* object)
{
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
IShape* RegionQuery::getShapeUnderPos(const QPoint& p) const
{
	rq::RQobjectPtr obj = m_tree->getObject(rq::CPoint(p));

	if (obj != nullptr)
		return obj->getObject();

	return nullptr;
}

std::vector<IShape*> RegionQuery::getShapesUnderRect(const QRect& oRect) const
{
	std::vector<IShape*> shapes;

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
