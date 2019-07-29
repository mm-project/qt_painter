#include "RegionQueryService.hpp"

#include "rq_object.hpp"

std::unique_ptr<RegionQuery> RegionQuery::m_instance = nullptr;

RegionQuery::RegionQuery()
{
	m_tree = std::shared_ptr<rq::RQtree<IShape>> (new rq::RQtree<IShape>());
}

RegionQuery& RegionQuery::getInstance()
{
	if (m_instance == nullptr)
		m_instance = std::unique_ptr<RegionQuery>(new RegionQuery);

	return *m_instance;
}

void RegionQuery::insertObject(IShape* object)
{
	rq::RQobjectPtr obj;
	switch (object->getType())
	{
	case IShape::Type::LINE:
		obj = std::shared_ptr<rq::IRQobject>(new rq::RQline(object));
		break;
	case IShape::Type::RECTANGLE:
		obj = std::shared_ptr<rq::IRQobject>(new rq::RQrect(object));
		break;
	case IShape::Type::ELLIPSE:
		obj = std::shared_ptr<rq::IRQobject>(new rq::RQellipse(object));
		break;
	case IShape::Type::POLYGON:
		obj = std::shared_ptr<rq::IRQobject>(new rq::RQpolygon(object));
		break;
	}

	m_tree->insert(obj);
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