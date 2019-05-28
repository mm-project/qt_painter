#include "RegionQueryService.hpp"

#include "rq_object.hpp"

std::unique_ptr<RegionQuery> RegionQuery::m_instance = nullptr;

RegionQuery::RegionQuery()
{
	m_tree = std::shared_ptr<rq::RQtree<IShape>> (new rq::RQtree<IShape>());
	//m_tree = std::shared_ptr < rq::RQtree <line>> (new rq::RQtree<line>());
}

RegionQuery& RegionQuery::getInstance()
{
	if (m_instance == nullptr)
		m_instance = std::unique_ptr<RegionQuery>(new RegionQuery);

	return *m_instance;
}

void RegionQuery::insertObject(IShape* object)
{
	rq::RQobjectPtr<line> obj = std::shared_ptr<rq::RQobject<line>>(new rq::RQobject<line>(object));

	m_tree->insert(obj);
}
 
IShape* RegionQuery::getShapeUnderPos(const QPoint& p) const
{
	rq::RQobjectPtr<IShape> obj = m_tree->getObject(rq::CPoint(p));

	if (obj != nullptr)
		return obj->getObject();

	return nullptr;
}