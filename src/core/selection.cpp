#include "selection.hpp"
#include "rq/RegionQueryService.hpp"

void Selection::clear() {
	highlight_on_off(false);
	WorkingSet::clear();
	m_sb->clear();
	//m_sb = 0;
	//delete m_sb;

	//highlight_on_off();
	//m_rt_pools->clear();
	
}

void Selection::set_working_set(IObjectPool* ws) {
	m_ws = ws;
	//m_h_on = false;
}

void Selection::set_sandbox(ObjectPoolSandbox* ops) {
	m_rt_pools = ops;
	m_sb = new ObjectSandbox();
	m_rt_pools->addChildren(std::shared_ptr<ObjectSandbox>(m_sb));
}

//asenq te
void Selection::select_and_highlight_shape_under_pos(const QPoint& p) {
	clear();
	highlight_on_off(false);


	//FIXME need only one init during shape creation
	RegionQuery& rq = RegionQuery::getInstance();
	for (auto obj : m_ws->getObjects())
		rq.insertObject(obj);

	IShape* shape = rq.getShapeUnderPos(p);
	if (shape != nullptr)
	{
		addObject(shape);
		highlight_on_off(true);
	}
	
	highlight_on_off(true);
}

void Selection::find_by_range_and_add_to_selected(const std::pair<QPoint,QPoint>& point) {
	if ( m_ws->getObjects().empty() )
		return;

	highlight_on_off(false);
	
	RegionQuery& rq = RegionQuery::getInstance();
	for (auto obj : m_ws->getObjects())
		rq.insertObject(obj);

	IShape* shape = rq.getShapeUnderPos(point.first);
	
	if (shape != nullptr )
	{
		addObject(shape);
		highlight_on_off(true);
	}

	//highlight_on_off(true);
}

void Selection::highlight_on_off(bool m_h_on) {
	//m_h_on = !m_h_on;
	if (getObjects().empty())
		return;
	
	//std::cout << "highlight: " << m_h_on << std::endl;
	
	//FIXME remember old propoerties by map
	ShapeProperties sp;
	if ( m_h_on )
		sp.pen_color = Qt::red;
	else
		sp.pen_color = Qt::white;
	
	for ( auto it: getObjects() ) {
		it->updateProperties(sp);
		m_sb->addObject(it);
	}
}
