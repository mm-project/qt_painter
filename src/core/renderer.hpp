#ifndef renderer_hpp
#define renderer_hpp

#include "iobject_pool.hpp"
#include "runtime_environment.hpp"
#include "postman.hpp"
#include "callback.hpp"

#include <QPainter>
#include <QWidget>
#include <QPoint>

#include <iostream>

enum panDirection { PANUP, PANDOWN, PANLEFT, PANRIGHT };

class canvasTransformClbkDt: public LeCallbackData
{
    public:
        canvasTransformClbkDt(float kx, int dx, float ky,int dy):m_kx(kx),m_dx(dx),m_ky(ky),m_dy(dy) {}
        
        float m_kx = 1;
        float m_ky = 1;
        int m_dx = 0;
        int m_dy = 0;
    
};

// REPONSIBLE FOR VIEWPORT CONTROLL
class renderer
{
	IObjectPoolPtr m_working_set;
	ObjectPoolSandboxPtr m_sandbox;
	int m_scale = 15;
        int m_pan_step = 15;
        QPoint m_origin_point = {0,0};
        bool m_need_adjustment = false;
        QRect* m_users_pov_rect;
 	
    //Q_OBJECT
    public:
        //WPainter(QPaintDevice* p):QPainter(p){
        renderer ( QWidget* w, ObjectPoolSandboxPtr r, IObjectPoolPtr s ):m_sandbox(r),m_working_set(s) { 	
            m_zoom_factor = 1;
            m_qt_painter = new QPainter(w);
            m_plane = w;
            m_users_pov_rect = new QRect(QPoint(0,0), m_plane->size());            
        }
        
        ~renderer() {
            delete m_qt_painter;
            m_qt_painter = 0;
        }
        
        //void drawRect(const QRect& r) {
        //	QRect r2(r);
        //	r2.setHeight(r.height());
        //	r2.setWidth(r.width());
        //	QPainter::drawRect(r2);
        //}
        
        void start() {
            m_qt_painter->begin(m_plane);
        }
        
        void stop() {
            m_qt_painter->end();
        }
        
        void pan(panDirection d) {
            switch (d) {
                case PANLEFT: m_origin_point.setX(m_origin_point.x()-m_pan_step); 
                            break;
                case PANRIGHT: m_origin_point.setX(m_origin_point.x()+m_pan_step); 
                            break;
                case PANUP: m_origin_point.setY(m_origin_point.y()-m_pan_step); 
                            break;
                case PANDOWN: m_origin_point.setY(m_origin_point.y()+m_pan_step); 
                            break;
            }
            notify_viewport_changed();    
        }

        void zoom(int factor, QPoint p ) {
                if ( factor > 0 )
                        zoomin_p(p);
                else
                        zoomout();
        }
                
        void zoomin_p(QPoint p) {
            m_origin_point = p;
            zoomin();
        }
        
        void zoomout_p(QPoint p) {
            m_origin_point = p;
            zoomout();
        }

        void zoomin() {
            m_zoom_factor*=2;
            notify_viewport_changed();

        }

        void zoomout() {
            std::cout << "zzomout" << m_zoom_factor << std::endl;
            if ( m_zoom_factor > 0.05 ) {
                    m_zoom_factor*=0.5;
                    notify_viewport_changed();
            }
        }

        void notify_viewport_changed() 
        {
            canvasTransformClbkDt d(m_zoom_factor,m_origin_point.x(),m_zoom_factor,m_origin_point.y());
            NOTIFY(CANVAS_VIEWPORT_CHANGED,d);
            m_need_adjustment = true;
        }
        
        int get_zoom_factor() const
        {
            return m_zoom_factor;
        }

        //void pan(int x, int y) {
        //	
        //}
        
        QPainter*  get_painter() {
            return m_qt_painter;
        }
        
        void draw_background() {
            m_qt_painter->setBrush(QBrush(Qt::black));
            m_qt_painter->drawRect(QRect(QPoint(m_origin_point.x(),m_origin_point.y()), m_plane->size()));
            //m_plane->setStyleSheet("background-color:black;");
        }

        void draw_grid() {
            QPen white(Qt::red);
            white.setWidth(1);
            white.setJoinStyle(Qt::RoundJoin);
            white.setCapStyle(Qt::RoundCap);
            int _height = -1*m_origin_point.y()+m_plane->size().height()-20;//m_plane->size().height();//m_origin_point.y()>0?m_plane->height()+m_origin_point.y():m_plane->height()-m_origin_point.y();
            int _width = -1*m_origin_point.x()+m_plane->size().width()-20; //m_plane->size().width();///m_pan_step0;//m_origin_point.x()>0?m_plane->width()+m_origin_point.x():m_plane->width()-m_origin_point.x();
            int startx = m_origin_point.x();
            int starty = m_origin_point.y();
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
        
        
        void draw_objects() {
                // draw working set
                std::vector<IShape*> shapes = m_working_set->getObjects();
                // fixme draw all objects in the bbox, from rq.
                // std::vector<IShape*> shapes = rq.getShapesUnderRect(m_users_pov_rect);
                for (auto i : shapes)
                        i->draw(m_qt_painter);
        }
        
        void make_viewport_adjustments() {
                //don't whant to scale and trasnform each time
                //if ( ! m_need_adjustment )
                //    return;
                
                //std::cout << "adjust!!! " << std::endl;
                //musers_pov_rect->setTopLeft(m_origin_point);
                m_qt_painter->scale(m_zoom_factor,m_zoom_factor);
                m_qt_painter->translate(QPoint(m_origin_point.x(),m_origin_point.y()));
                //m_users_pov_rect->adjust(0,0,-1*m_origin_point.y(),-1*m_origin_point.x());
                m_need_adjustment = false;
        }
            
        void draw_runtime_pools() {
                // draw runtime
            auto pools = m_sandbox->getChildren();
            for (auto it : pools)
            {
                if (it == nullptr)
                                continue;

                auto p = it->getPool();
                if (p == nullptr)
                                continue;

                auto objs = p->getObjects();
                for (auto i : objs)
                        i->draw(m_qt_painter);
            }
        }
        
        void draw_all() {
                draw_background();
                draw_objects();
                draw_runtime_pools();
                draw_grid();            
        }

        void render() {
                start();
                make_viewport_adjustments();
                draw_all();
                stop();
        }
		
		
    private:
            float m_zoom_factor;
            
    private:
            QPainter* m_qt_painter;
            QWidget* m_plane;
};

#endif
