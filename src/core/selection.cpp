#include "selection.hpp"
#include "postman.hpp"

#include "../gui/controller.hpp"

#include "rq/RegionQueryService.hpp"

#include <iostream>

std::string Selection::getName() 
{
        return "Selection";
}

void Selection::clear() {
	WorkingSet::clear();
        m_sel_highlight_set->clear();
	m_oa_highlight_set->clear();
        m_sb->clear();
}

void Selection::set_working_set(IObjectPool* ws) {
	m_ws = ws;        
	//m_h_on = false;
        REGISTER_CALLBACK(CONTROLLER_CHANGED,&Selection::on_controller_update);
}

void Selection::on_controller_update(LeCallbackData&) {
        std::cout << "changed.." << std::endl;
        
        if ( m_ws->getObjects().empty() ||  getObjects().empty() )
            return;
            
        for ( auto obj : m_sb->getPool()->getObjects() ) {
            obj->updateProperties(controller::get_instance()->get_shape_properties());
            get_clonee(obj)->updateProperties(controller::get_instance()->get_shape_properties());        
        }
}

void Selection::set_sandbox(ObjectPoolSandbox* ops) {
	m_rt_pools = ops;
	m_sb = new ObjectSandbox();
	m_rt_pools->addChildren(std::shared_ptr<ObjectSandbox>(m_sb));
        
        ShapeProperties p1;
		p1.pen_color = Qt::yellow;
                p1.brush_color = Qt::blue;
                p1.pen_style = Qt::DotLine;
                p1.brush_style = Qt::DiagCrossPattern;
                p1.pen_width = 5;
                
        ShapeProperties p2;
		p2.pen_color = Qt::red;
                p2.pen_style = Qt::DashLine;
                p2.pen_width = 2;
        
        m_oa_highlight_set = new HighlightSet("ActiveObjects",p1);
        m_sel_highlight_set = new HighlightSet("Selection",p2);
        m_sel_highlight_set->create_sandbox(ops);
        m_oa_highlight_set->create_sandbox(ops);
}

//asenq te
void Selection::highlightselect_shape_under_pos(const QPoint& p) {
	clear();
        RegionQuery& rq = RegionQuery::getInstance();
	IShape* shape = rq.getShapeUnderPos(p);
	if (shape != nullptr)
	{
		addObject(shape);
		m_sel_highlight_set->addObject(shape);
                m_sel_highlight_set->highlight_on();
	}
}


void Selection::highlight_shape_under_pos(const QPoint& p) {
	m_oa_highlight_set->clear();
        RegionQuery& rq = RegionQuery::getInstance();
	IShape* shape = rq.getShapeUnderPos(p);
	if (shape != nullptr)
	{
		m_oa_highlight_set->addObject(shape);
                m_oa_highlight_set->highlight_on();
	}
}


void Selection::find_and_highlightselect_shapes_from_region(const std::pair<QPoint,QPoint>& point) {
        clear();
        if ( m_ws->getObjects().empty() )
		return;

        RegionQuery& rq = RegionQuery::getInstance();
        for (auto it : rq.getShapesUnderRect(QRect(point.first, point.second))) {
		addObject(it);
                m_sel_highlight_set->addObject(it);
        }
        
	m_sel_highlight_set->highlight_on();

}


//*********************************** HIGHLIGHT ********************************************
HighlightSet::HighlightSet(const std::string& n,const ShapeProperties& p ):m_name(n),m_packet(p) {
    
}


void HighlightSet::create_sandbox(ObjectPoolSandbox* ops) {
    m_rt_pools = ops;
    m_sb = new ObjectSandbox();
    m_rt_pools->addChildren(std::shared_ptr<ObjectSandbox>(m_sb));
}

std::string HighlightSet::getName() {
    return m_name;
}

void HighlightSet::highlight_on()
{
    highlight_on_off(true);
}

void HighlightSet::clear() {
    WorkingSet::clear();
    highlight_off();
}

void HighlightSet::highlight_off()
{
    m_sb->clear();
    //highlight_on_off(false);    
}

void HighlightSet::highlight_on_off(bool m_h_on) {
	if (getObjects().empty())
		return;
	
	for ( auto it: getObjects() ) {
		it->updateProperties(m_packet);
		m_sb->addObject(it);
	}
}
