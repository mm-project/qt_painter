#include "renderer.hpp"
#include "core.hpp"

#include "../commands/canvas_commands.hpp"

renderer::renderer ( QWidget* w, RuntimePoolManagerPtr r, ObjectPoolPtr s ):m_sandbox(r),m_working_set(s) { 	
    m_scale_factor = 1;
    m_qt_painter = new QPainter(w);
    m_plane = w;
    m_users_pov_rect = new QRect(QPoint(0,0), m_plane->size());            
}

renderer::~renderer() {
    delete m_qt_painter;
    m_qt_painter = 0;
}

//void renderer::drawRect(const QRect& r) {
//	QRect r2(r);
//	r2.setHeight(r.height());
//	r2.setWidth(r.width());
//	QPainter::drawRect(r2);
//}

void renderer::start() {
    m_qt_painter->begin(m_plane);
}

void renderer::stop() {
    m_qt_painter->end();
}


void renderer::set_cursor_pos_for_drawing(int x,int y) 
{
	c_cursor_x = x/m_scale_factor - m_origin_point.x();
	c_cursor_y = y/m_scale_factor - m_origin_point.y();
}

void renderer::pan(const panDirection& d) {
    
    switch (d) {
        case PANLEFT: m_origin_point.setX(m_origin_point.x()-m_pan_step); 
                    dicmdCanvasOrigin<PANLEFT>().log();
                    break;
        case PANRIGHT: m_origin_point.setX(m_origin_point.x()+m_pan_step); 
                    dicmdCanvasOrigin<PANRIGHT>().log();
                    break;
        case PANUP: m_origin_point.setY(m_origin_point.y()-m_pan_step); 
                    dicmdCanvasOrigin<PANUP>().log();
                    break;
        case PANDOWN: m_origin_point.setY(m_origin_point.y()+m_pan_step); 
                    dicmdCanvasOrigin<PANDOWN>().log();
                    break;
    }
    notify_viewport_changed();    
}

void renderer::zoom_internal(const zoomDirection& z, QPoint p ) {
        if ( z == ZOOMIN )
            zoomin_p(p);
        else
            zoomout_p(p);
}

void renderer::zoom(int factor, QPoint p ) {
        if ( factor > 0 )
            zoom_internal(ZOOMIN,p);
        else
            zoom_internal(ZOOMOUT,p);
}


void renderer::prezoom(QPoint p) {
    
    
}

void renderer::zoomin_p(QPoint p) {
    //std::cout << "in!!!!!!!!!!!!!!!!!!!!" << p.x() << " " << p.y() << std::endl;
    //m_origin_point = p;
    dicmdCanvasViewport<ZOOMIN>(p).log();
    zoomin();
    //std::cout << "NNNNN!!!!!!!!!!!!!!!!!!!!" << m_origin_point.x() << " " << m_origin_point.y() << std::endl;
    notify_viewport_changed();
}


void renderer::zoomout_p(QPoint p) {
    //std::cout << "OU!!!!!!!!!!!!!!!!!!!!" << p.x() << " " << p.y() << std::endl;
    //m_origin_point = m_origin_point - p;
    //m_origin_point = m_origin_point+m_scale_factor*p;
    dicmdCanvasViewport<ZOOMOUT>(p).log();
    zoomout();
    //m_origin_point = p;
    notify_viewport_changed();
}

void renderer::zoomin() {
    m_scale_factor*=m_zoom_factor;
    //notify_viewport_changed();

}

void renderer::zoomout() {
    //std::cout << "zzomout" << m_scale_factor << std::endl;
    if ( m_scale_factor > 0.05 ) {
            m_scale_factor/=m_zoom_factor;
            //notify_viewport_changed();
    }
}

void renderer::notify_viewport_changed() 
{
    canvasTransformClbkDt d(m_scale_factor,m_origin_point.x(),m_scale_factor,m_origin_point.y());
    NOTIFY(CANVAS_VIEWPORT_CHANGED,d);
    m_need_adjustment = true;
}

float renderer::get_zoom_factor() 
{
    return m_scale_factor;
}

//void renderer::pan(int x, int y) {
//	
//}

QPainter*  renderer::get_painter() {
    return m_qt_painter;
}

void renderer::draw_background() {
    m_qt_painter->setBrush(QBrush(Qt::black));
    QSize s = m_plane->size();
    auto _h = s.height();
    auto _w = s.width();            
    int w = _w/m_scale_factor; //m_scale_factor>1?m_scale_factor/_w:m_scale_factor/_w;
    int h = _h/m_scale_factor; //m_scale_factor>1?m_scale_factor/_h:m_scale_factor*_h;
    int x = -1*m_origin_point.x();//m_origin_point.x() > 0 ? -1*m_origin_point.x() : m_origin_point.x();
    int y = -1*m_origin_point.y(); //m_origin_point.y() > 0 ? -1*m_origin_point.y() : m_origin_point.y();
    
    m_qt_painter->drawRect(QRect(QPoint(x,y), QSize(w,h)));
    //m_plane->setStyleSheet("background-color:black;");
}

//broken
//*
void renderer::draw_grid() {
    //m_qt_painter->scale(1,1);
    //m_qt_painter->translate(QPoint(0,0));

    QPen white(Qt::red);
    white.setWidth(1);
    white.setJoinStyle(Qt::RoundJoin);
    white.setCapStyle(Qt::RoundCap);
    
    //std::cout <<   1/get_zoom_factor() << std::endl;
    int _height =  1/get_zoom_factor()*(m_plane->height());//m_origin_point.y()>0?m_plane->height()+m_origin_point.y():m_plane->height()-m_origin_point.y();
    int _width = 1/get_zoom_factor()*(m_plane->width());//*m_origin_point.x()+m_plane->size().width()-20; //m_plane->size().width();///m_pan_step0;//m_origin_point.x()>0?m_plane->width()+m_origin_point.x():m_plane->width()-m_origin_point.x();
    int startx = m_origin_point.x();;
    int starty = m_origin_point.y();
    
    //std::cout << startx << " " << _width << "      " << starty << " " << _height << std::endl;
    for (int i = startx, _i = startx; i < _width; i += m_scale, ++_i)
            for (int j = starty, _j = starty; j < _height; j += m_scale, ++_j)
            {
                    if ((_i % 5 == 0) && (_j % 5 == 0))
                            white.setWidth(3);
                    
                    white.setJoinStyle(Qt::RoundJoin);
                    white.setCapStyle(Qt::RoundCap);
                    m_qt_painter->setPen(white);
                    m_qt_painter->drawPoint(i, j);
                    white.setWidth(1);
            }
}
/**/

void renderer::draw_objects() {
        // draw working set
        std::vector<IShapePtr> shapes = m_working_set->getObjects();
        // fixme draw all objects in the bbox, from rq.
        // std::vector<IShape*> shapes = rq.getShapesUnderRect(m_users_pov_rect);
        for (auto i : shapes)
			if (i != nullptr)
				i->draw(m_qt_painter);
}

void renderer::make_viewport_adjustments() {
        //don't whant to scale and trasnform each time
        //if ( ! m_need_adjustment )
        //    return;
        
        //std::cout << "adjust!!! " << std::endl;
        //musers_pov_rect->setTopLeft(m_origin_point);
        m_qt_painter->scale(m_scale_factor,m_scale_factor);
        m_qt_painter->translate(m_origin_point);
        //m_users_pov_rect->adjust(0,0,-1*m_origin_point.y(),-1*m_origin_point.x());
        m_need_adjustment = false;
}
    
void renderer::draw_runtime_pools() {
        // draw runtime
    auto pools = m_sandbox->getChildren();
    for (auto it : pools)
    {
		ASSERT_CONTINUE(it.second != nullptr);
        auto objs = it.second->getObjects();
        for (auto i : objs)
			i->draw(m_qt_painter);
    }
}

void renderer::click_hint() {
    m_need_draw_clicked = true;
}

void renderer::draw_cursor() {

	QPen p;
	if ( m_need_draw_clicked ){
		p.setColor(Qt::red);
                p.setWidth(12);
        } else {
		p.setColor(Qt::blue);
                p.setWidth(6);
        }
        
        //p.setJoinStyle(Qt::RoundJoinStyle);
	//p.setCapStyle(Qt::RoundCapStyle);

	m_qt_painter->setPen(p);
	m_qt_painter->drawPoint(c_cursor_x, c_cursor_y);
	m_need_draw_clicked = false;
}

void renderer::draw_all() {
        draw_background();
        draw_grid();            
        draw_objects();
        draw_runtime_pools();
        draw_cursor();
}

void renderer::draw_all_wno_cursor() {
        draw_background();
        draw_grid();            
        draw_objects();
        draw_runtime_pools();
}

void renderer::render() {
        start();
        make_viewport_adjustments();
        draw_all();
        stop();
}
		
		
