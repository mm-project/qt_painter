#include "renderer.hpp"
#include "../commands/canvas_commands.hpp"
#include "application.hpp"
#include "core.hpp"
#include "rq/RegionQueryService.hpp"

#include <cassert>

renderer::renderer(QWidget *w, RuntimePoolManagerPtr r, ObjectPoolPtr s) : m_sandbox(r), m_working_set(s)
{
    m_scale_factor = 1;
    m_qt_painter = new QPainter(w);
    m_plane = w;
    m_users_pov_rect = new QRect(QPoint(0, 0), m_plane->size());
}

renderer::~renderer()
{
    delete m_qt_painter;
    m_qt_painter = 0;
}

// void renderer::drawRect(const QRect& r) {
//	QRect r2(r);
//	r2.setHeight(r.height());
//	r2.setWidth(r.width());
//	QPainter::drawRect(r2);
// }

void renderer::start()
{
    m_qt_painter->begin(m_plane);
}

void renderer::stop()
{
    m_qt_painter->end();
}

void renderer::set_cursor_pos_for_drawing(int x, int y)
{
    c_cursor_x = x / m_scale_factor - m_origin_point.x();
    c_cursor_y = y / m_scale_factor - m_origin_point.y();
    // assert(0);
}

void renderer::pan(const panDirection &d)
{

    switch (d)
    {
    case PANLEFT:
        m_origin_point.setX(m_origin_point.x() - m_pan_step);
        dicmdCanvasOrigin<PANLEFT>().log();
        break;
    case PANRIGHT:
        m_origin_point.setX(m_origin_point.x() + m_pan_step);
        dicmdCanvasOrigin<PANRIGHT>().log();
        break;
    case PANUP:
        m_origin_point.setY(m_origin_point.y() - m_pan_step);
        dicmdCanvasOrigin<PANUP>().log();
        break;
    case PANDOWN:
        m_origin_point.setY(m_origin_point.y() + m_pan_step);
        dicmdCanvasOrigin<PANDOWN>().log();
        break;
    }
    notify_viewport_changed();
}

void renderer::zoom_internal(const zoomDirection &z, QPoint p)
{
    if (z == ZOOMIN)
        zoomin_p(p);
    else
        zoomout_p(p);
}

void renderer::zoom(int factor, QPoint p)
{
    if (factor > 0)
        zoom_internal(ZOOMIN, p);
    else
        zoom_internal(ZOOMOUT, p);
}

void renderer::prezoom(QPoint p)
{
}

void renderer::zoomin_p(QPoint p)
{
    // std::cout << "in!!!!!!!!!!!!!!!!!!!!" << p.x() << " " << p.y() << std::endl;
    // m_origin_point = p;
    dicmdCanvasViewport<ZOOMIN>(p).log();
    zoomin();
    // std::cout << "NNNNN!!!!!!!!!!!!!!!!!!!!" << m_origin_point.x() << " " << m_origin_point.y() << std::endl;
    notify_viewport_changed();
}

void renderer::zoomout_p(QPoint p)
{
    // std::cout << "OU!!!!!!!!!!!!!!!!!!!!" << p.x() << " " << p.y() << std::endl;
    // m_origin_point = m_origin_point - p;
    // m_origin_point = m_origin_point+m_scale_factor*p;
    dicmdCanvasViewport<ZOOMOUT>(p).log();
    zoomout();
    // m_origin_point = p;
    notify_viewport_changed();
}

void renderer::zoomin()
{
    if (m_scale_factor < 10)
        m_scale_factor *= m_zoom_factor;
    // notify_viewport_changed();
}

void renderer::zoomout()
{
    // std::cout << "zzomout" << m_scale_factor << std::endl;
    if (m_scale_factor > 0.005)
    {
        m_scale_factor /= m_zoom_factor;
        // notify_viewport_changed();
    }
}

void renderer::notify_viewport_changed()
{
    canvasTransformClbkDt d(m_scale_factor, m_origin_point.x(), m_scale_factor, m_origin_point.y());
    NOTIFY(CANVAS_VIEWPORT_CHANGED, d);
    m_need_adjustment = true;
}

float renderer::get_zoom_factor()
{
    return m_scale_factor;
}

// void renderer::pan(int x, int y) {
//
// }

QPainter *renderer::get_painter()
{
    return m_qt_painter;
}

void renderer::draw_background()
{
    m_qt_painter->setBrush(QBrush(Qt::black));
    QSize s = m_plane->size();
    auto _h = s.height();
    auto _w = s.width();
    int w = _w / m_scale_factor;     // m_scale_factor>1?m_scale_factor/_w:m_scale_factor/_w;
    int h = _h / m_scale_factor;     // m_scale_factor>1?m_scale_factor/_h:m_scale_factor*_h;
    int x = -1 * m_origin_point.x(); // m_origin_point.x() > 0 ? -1*m_origin_point.x() : m_origin_point.x();
    int y = -1 * m_origin_point.y(); // m_origin_point.y() > 0 ? -1*m_origin_point.y() : m_origin_point.y();

    m_qt_painter->drawRect(QRect(QPoint(x, y), QSize(w, h)));
    // m_plane->setStyleSheet("background-color:black;");
}

// broken
//*
void renderer::draw_grid()
{
    // m_qt_painter->scale(1,1);
    // m_qt_painter->translate(QPoint(0,0));

    QPen white(Qt::red);
    white.setWidth(1);
    white.setJoinStyle(Qt::RoundJoin);
    white.setCapStyle(Qt::RoundCap);

    int _height = 1 / get_zoom_factor() * (m_plane->height()) - m_origin_point.y();
    int _width = 1 / get_zoom_factor() * (m_plane->width()) - m_origin_point.x();
    int startx = 1 / get_zoom_factor() * (m_old_origin_point.x() - m_origin_point.x()) - m_old_origin_point.x();
    int starty = 1 / get_zoom_factor() * (m_old_origin_point.y() - m_origin_point.y()) - m_old_origin_point.y();

    // std::cout << startx << " " << _width << "      " << starty << " " << _height << std::endl;
    for (int i = startx, _i = startx; i < _width; i += m_scale, ++_i)
        for (int j = starty, _j = starty; j < _height; j += m_scale, ++_j)
        {
            if ((_i % 4 == 0) && (_j % 4 == 0))
                white.setWidth(5);

            white.setJoinStyle(Qt::RoundJoin);
            white.setCapStyle(Qt::RoundCap);
            m_qt_painter->setPen(white);
            m_qt_painter->drawPoint(i, j);
            white.setWidth(1);
        }
}
/**/

void renderer::draw_objects()
{
    // draw working set
    // std::vector<IShape*> shapes = m_working_set->getObjects();
    // fixme draw all objects in the bbox, from rq.
    // std::vector<IShape*> shapes = rq.getShapesUnderRect(m_users_pov_rect);
    // for (auto i : shapes)
    //       i->draw(m_qt_painter);

    int _height = 1 / get_zoom_factor() * (m_plane->height());
    int _width = 1 / get_zoom_factor() * (m_plane->width());
    int startx = -1 * m_origin_point.x(); // m_old_origin_point.x()-m_origin_point.x();
    int starty = -1 * m_origin_point.y();
    // std::cout << "renderer" << startx << " " << starty << "      " << _width << " " << _height << std::endl;

    if (m_rq_renderer)
    {
        RegionQuery &rq = RegionQuery::getInstance();
        for (auto shape : rq.getShapesUnderRect(QRect(startx, starty, _width, _height)))
            shape->draw(m_qt_painter);
    }
    else
    {
        for (auto i : m_working_set->getObjects())
            i->draw(m_qt_painter);
    }
}

void renderer::make_viewport_adjustments()
{
    // don't whant to scale and trasnform each time
    // if ( ! m_need_adjustment )
    //     return;

    // std::cout << "adjust!!! " << std::endl;
    // musers_pov_rect->setTopLeft(m_origin_point);
    m_qt_painter->scale(m_scale_factor, m_scale_factor);
    m_qt_painter->translate(m_origin_point);
    // m_users_pov_rect->adjust(0,0,-1*m_origin_point.y(),-1*m_origin_point.x());
    m_need_adjustment = false;
    m_old_origin_point = m_origin_point;
}

void renderer::draw_runtime_pools()
{
    // draw runtime
    auto pools = m_sandbox->getChildren();
    for (auto it : pools)
    {
        ASSERT_CONTINUE(it.second != nullptr);
        auto objs = it.second->getObjects();
        // std::cout << it.first << "  : " << objs.size() << std::endl;
        for (auto i : objs)
            i->draw(m_qt_painter);
    }
}

void renderer::click_hint()
{
    m_need_draw_clicked = true;
}

void renderer::draw_cursor()
{

    QPen p;
    if (m_need_draw_clicked)
    {
        p.setColor(Qt::red);
        p.setWidth(12);
    }
    else
    {
        p.setColor(Qt::blue);
        p.setWidth(10);
    }

    // p.setJoinStyle(Qt::RoundJoinStyle);
    // p.setCapStyle(Qt::RoundCapStyle);

    m_qt_painter->setPen(p);
    m_qt_painter->drawPoint(c_cursor_x, c_cursor_y);
    m_need_draw_clicked = false;
}
/*
void renderer::draw_selection_rubberband()
{
    QPen p;
    p.setColor(Qt::red);
    m_qt_painter->setPen(p);

    for ( auto obj: m_se.getObjects() )
            obj->draw(m_qt_painter);
}
*/


QPixmap renderer::put_shapes_on_pixmap(auto shapes, int i, int w, int h)
{
    QPixmap pixmap(w,h);
    QColor color(255/(1+i), 255/(1+i), 255/(1+i));
    pixmap.fill(color);

    QPainter *painter = new QPainter(&pixmap);
    for (const auto& shape : shapes)
        shape->draw(painter);

    pixmap.save("region" + QString::number(i) + ".bmp");

    delete painter;
    return pixmap;

}

std::vector<QRect> renderer::init_query_rects(int num_regions)
{

    std::vector<QRect> query_rects(num_regions);

    int factor;
    switch(num_regions) {
        case 4:
            factor = 2;
            break;
        case 16:
            factor = 4;
            break;
        case 64:
            factor = 8;
            break;
    }


    int canvas_h = 1 / get_zoom_factor() * (m_plane->height()) - m_origin_point.y();
    int canvas_w = 1 / get_zoom_factor() * (m_plane->width()) - m_origin_point.x();

    int query_w = canvas_w/factor;
    int query_h = canvas_h/factor;

    int k=0;
    for(int i=0;i<factor;i++)
        for(int j=0;j<factor;j++) {
            std::cout << "creating region " << k << std::endl;
            query_rects[k] = QRect(j*canvas_w/factor,i*canvas_h/factor,query_w,query_h);
            k++;
        }

    return query_rects;
}

void renderer::draw_all()
{
    RegionQuery &rq = RegionQuery::getInstance();

    int canvas_h = 1 / get_zoom_factor() * (m_plane->height()) - m_origin_point.y();
    int canvas_w = 1 / get_zoom_factor() * (m_plane->width()) - m_origin_point.x();

    int num_threads = 4;
    int num_regions = num_threads;
    int factor;
    switch(num_regions) {
        case 4:
            factor = 2;
            break;
        case 16:
            factor = 4;
            break;
        case 64:
            factor = 8;
            break;
    }

    int query_w = canvas_w/factor;
    int query_h = canvas_h/factor;

    std::vector<QRect> query_rects = init_query_rects(num_regions);
    std::vector<QPixmap> pixmaps(num_regions);

    #pragma omp parallel for
    for(int i=0;i<num_regions;i++){
        auto shapes = rq.getShapesUnderRect(query_rects[i]);
        //std::cout << "region" << i << ":   shapes:" << shapes.size() << std::endl;
        pixmaps[i] = put_shapes_on_pixmap(shapes,i,canvas_w,canvas_h);
    }

    QPixmap canvas(canvas_w,canvas_h);
    canvas.fill(Qt::black);
    //QPainter *painter = new QPainter(&canvas);
    m_qt_painter->setPen(QColor(255,34,255,255));
    #pragma omp parallel for
    for(int i=0;i<num_regions;i++)
        m_qt_painter->drawPixmap(query_rects[i].x(),query_rects[i].y(),query_w,query_h,pixmaps[i],query_rects[i].x(),query_rects[i].y(),query_w,query_h);
    //canvas.save("canvas.bmp");
    //delete painter;
}

/*
void renderer::draw_all()
{
    RegionQuery &rq = RegionQuery::getInstance();
    //int canvas_w = m_plane->width();
    //int canvas_h = m_plane->height();

    int canvas_h = 1 / get_zoom_factor() * (m_plane->height()) - m_origin_point.y();
    int canvas_w = 1 / get_zoom_factor() * (m_plane->width()) - m_origin_point.x();

    //int canvas_h = get_zoom_factor() * (m_plane->height());
    //int canvas_w = get_zoom_factor() * (m_plane->width());

    int query_w = canvas_w/2;
    int query_h = canvas_h/2;
    int mid_x = canvas_w/2;
    int mid_y = canvas_h/2;

    int regions = 4;
    query_regions = init_regions(regions)
    std::vector<QPixmap> regions(regions);
    #pragma omp parallel for
    for(int i=0;i<regions;i++){
        auto shapes = rq.getShapesUnderRect(query_regions[i]);
        std::cout << "region" << i << ":   shapes:" << shapes.size() << std::endl;
        regions[i] = get_shapes_on_pixmap(shapes,i,canvas_w,canvas_h);
    }

    QPixmap canvas(canvas_w,canvas_h);
    canvas.fill(Qt::black);
    //QPainter *painter = new QPainter(&canvas);
    m_qt_painter->setPen(QColor(255,34,255,255));
    m_qt_painter->drawPixmap(0,0,query_w,query_h,regions[0],0,0,query_w,query_h);
    m_qt_painter->drawPixmap(mid_x,0,query_w,query_h,regions[1],mid_x,0,query_w,query_h);
    m_qt_painter->drawPixmap(0,mid_y,query_w,query_h,regions[2],0,mid_y,query_w,query_h);
    m_qt_painter->drawPixmap(mid_x,mid_y,query_w,query_h,regions[3],mid_x,mid_y,query_w,query_h);

    //draw_grid();

    /*
    draw_background();
    draw_grid();
    if (m_des_renderer)
        draw_objects();
    if (m_rt_renderer)
        draw_runtime_pools();
    if (Application::is_replay_mode())
        draw_cursor();
    */
//}
//*/

void renderer::draw_all_wno_cursor()
{
    draw_background();
    draw_grid();
    draw_objects();
    draw_runtime_pools();
    // draw_selection_rubberband();
}

void renderer::render()
{
    start();
    make_viewport_adjustments();
    draw_all();
    stop();
}

void renderer::rendering_des_mode_change()
{
    // assert(0);
    m_des_renderer = !m_des_renderer;
    std::cout << " DS RENDERING: " << m_des_renderer << std::endl;
}

void renderer::rendering_mode_change()
{
    m_rq_renderer = !m_rq_renderer;
    if (m_rq_renderer)
        std::cout << " RENDERING: RQ " << m_rq_renderer << std::endl;
    else
        std::cout << " RENDERING: WS " << m_rq_renderer << std::endl;
}

void renderer::rendering_rt_mode_change()
{
    m_rt_renderer = !m_rt_renderer;
    std::cout << " RT RENDERING: " << m_rt_renderer << std::endl;
}
