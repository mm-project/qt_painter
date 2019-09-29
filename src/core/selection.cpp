#include "selection.hpp"
//#include "postman.hpp"
#include "shapes.hpp"
#include "rq/RegionQueryService.hpp"

//#include <csignal>

std::string Selection::getName() 
{
    //int* a;
    //*a = 1;    
    //std::raise(SIGSEGV);
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
}

void Selection::set_sandbox(ObjectPoolSandbox* sanboxes) {
	m_rt_pools = sanboxes;
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

        ShapeProperties p3;
		p3.pen_color = Qt::white;
                p3.brush_style = Qt::NoBrush;
                p3.pen_style = Qt::DashDotLine;
                p3.pen_width = 1;

        m_oa_highlight_set = new HighlightSet("ActiveObjects",p1);
        m_sel_highlight_set = new HighlightSet("Selection",p2);
        m_qa_highlight_set = new HighlightSet("QASelectionsTintirid",p3);
        
        m_sel_highlight_set->create_sandbox(m_rt_pools);
        m_oa_highlight_set->create_sandbox(m_rt_pools);
        m_qa_highlight_set->create_sandbox(m_rt_pools);
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
        
        m_last_region = QRect(point.first, point.second);
	m_sel_highlight_set->highlight_on();

}


void Selection::highlight_last_selected_region(bool on_off) 
{
    //std::cout << "hiiiiiiiiiiiiiiiiiiiiii" << std::endl;
    if ( ! on_off ) {
        m_qa_highlight_set->clear();
        m_last_region = QRect(0,0,0,0);
        return;
    }
    
    std::cout << m_last_region.bottomRight().x() << " " << m_last_region.bottomRight().y() << std::endl;
    m_qa_highlight_set->addObject(rectangle(m_last_region, ShapeProperties()).clone());
    m_qa_highlight_set->highlight_on();
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

