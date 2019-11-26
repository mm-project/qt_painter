#ifndef renderer_hpp
#define renderer_hpp

#include "iobject_pool.hpp"
#include "runtime_environment.hpp"

#include <QPainter>
#include <QWidget>
#include <QPoint>

// REPONSIBLE FOR VIEWPORT CONTROLL
class renderer
{
	IObjectPoolPtr m_working_set;
	ObjectPoolSandboxPtr m_sandbox;
	int m_scale = 15;
    QPoint m_zoom_point = {0,0};
	
    //Q_OBJECT
    public:
        //WPainter(QPaintDevice* p):QPainter(p){
        renderer ( QWidget* w, ObjectPoolSandboxPtr r, IObjectPoolPtr s ):m_sandbox(r),m_working_set(s) { 	
            m_zoom_factor = 1;
            painter = new QPainter(w);
            parent_windget = w;
        }
        
        ~renderer() {
                
        }
        
        //void drawRect(const QRect& r) {
        //	QRect r2(r);
        //	r2.setHeight(r.height());
        //	r2.setWidth(r.width());
        //	QPainter::drawRect(r2);
        //}
        
        void start() {
            painter->begin(parent_windget);
        }
        
        void stop() {
            painter->end();
        }
        
        void zoomin(QPoint p) {
            m_zoom_factor*=2;
            m_zoom_point = p;
            
        }
        
        void zoomout() {
            m_zoom_point = QPoint(0,0);
            if ( m_zoom_factor > 1 )
				m_zoom_factor/=2;
        }

		int get_zoom_factor() const
		{
			return m_zoom_factor;
		}

        //void pan(int x, int y) {
        //	
        //}
        
        QPainter*  get_painter() {
            return painter;
        }
        
		void draw_background() {
			QRect rect(m_zoom_point, parent_windget->size());
			QBrush b(Qt::black);
			painter->setBrush(b);
			painter->drawRect(rect);
		}

		void draw_grid() {
			QPen white(Qt::white);
			white.setWidth(1);
			white.setJoinStyle(Qt::RoundJoin);
			white.setCapStyle(Qt::RoundCap);
			painter->setPen(white);
			int _height = parent_windget->height();
			int _width = parent_windget->width();
			for (int i = 0, _i  = 0; i < _width; i += m_scale, ++_i)
				for (int j = 0, _j = 0; j < _height; j += m_scale, ++_j)
				{
					if ((_i % 5 == 0) && (_j % 5 == 0))
						white.setWidth(4);
					white.setJoinStyle(Qt::RoundJoin);
					white.setCapStyle(Qt::RoundCap);
					painter->setPen(white);
					painter->drawPoint(i, j);
					white.setWidth(1);
				}
		}
		
		
		void draw_runtime_pools() {
			// draw working set
			std::vector<IShape*> shapes = m_working_set->getObjects();
			for (auto i : shapes)
				i->draw(painter);
		}
		
		void make_viewport_adjustments() {
			painter->scale(m_zoom_factor,m_zoom_factor);
            painter->translate(QPoint(-1*m_zoom_point.x(),-1*m_zoom_point.y()));
        }
        
		void draw_all() {
            draw_background();
			draw_grid();
			draw_runtime_pools();
            draw_objects();
		}
		
		void draw_objects() {
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
							i->draw(painter);
			}
		}
		
		void render() {
			start();
                        make_viewport_adjustments();
			draw_all();
			stop();
		}
		
		void zoom(int factor, QPoint p ) {
			if ( factor > 0 )
				zoomin(p);
			else
				zoomout();
		}
		
    private:
            int m_zoom_factor;
            
    private:
            QPainter* painter;
            QWidget* parent_windget;
};

#endif
