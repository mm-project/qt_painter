#include "RegionQueryService.hpp"
#include "rq_object.hpp"
#include "../postman.hpp"
#include "../../io/messenger.hpp"

std::unique_ptr<RegionQuery> RegionQuery::m_instance = nullptr;

RegionQuery::RegionQuery()
{
	m_tree = std::shared_ptr<rq::RQtree<IShape>> (new rq::RQtree<IShape>());
    REGISTER_CALLBACK(DB_SHAPE_ADDED,&RegionQuery::insertObject);
    //nagaina to-do :* :D
    //REGISTER_CALLBACK(DB_SHAPE_WILLBE_DELETED,&RegionQuery::removeObject);
}

RegionQuery& RegionQuery::getInstance()
{
	if (m_instance == nullptr)
		m_instance = std::unique_ptr<RegionQuery>(new RegionQuery);

	return *m_instance;
}

void RegionQuery::insertObject(LeCallbackData& d)
{
	IShape* object(dynamic_cast<WSCallbackData&>(d).get_data());
    if (!object) {
        Messenger::expose_msg(err,"internal error with RQ when inserting data.");
        return;
    }
    
    std::cout << "!~" << object << std::endl;
    
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
