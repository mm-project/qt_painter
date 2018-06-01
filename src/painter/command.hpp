#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <QWidget>
#include <QPainter>
#include <QPaintDevice>

#include "controller.hpp"
#include "runtime_environment.hpp"
#include "working_set.hpp"

class basecommand
{
	public:
		void virtual execute() {
			//while ( isFinished() ) {
			//	process();
			//}
		}
		
		//bool virtual isFinished() = 0;
		void virtual process() = 0;
		void virtual mouse_clicked(int, int) = 0;
		void virtual mouse_move(int, int) = 0;
		
	
};

class create_rectangle : public basecommand
{
	public:
		create_rectangle(runtime_environment* r,working_set* s):re(r),ws(s) {
			m_is_first_click = true;
			m_finished = false;
			re->change_object_type(RECT);
		}
		
		//virtual bool isFinished() {
		//	return m_finished;
		//}
		
		virtual void process() {
			m_finished = true;
		}
		
		virtual void  mouse_clicked(int x, int y) {
			if ( m_is_first_click ) { 
                controller* c = controller::get_instance();
                re->change_basic_properties(c->get_basic_properties());
                re->set_pos1(QPoint(x,y));
                m_is_first_click = false;
			} else {
				re->set_pos2(QPoint(x,y));
				m_finished = true;
                ws->add_object(re);
                re->reset();
			}
		}
		
		virtual void  mouse_move(int x, int y) {
			if ( m_finished ) return;
			re->set_pos2(QPoint(x,y));
		}
		
		//void virtual 
		
	private:
		bool m_is_first_click;
		bool m_finished;
		runtime_environment* re;
		working_set* ws;
};




class command_manager  {
	
	public:
		
		command_manager(runtime_environment* r, working_set* s):re(r),ws(s) {
				
		}
	
		basecommand* get_create_rectangle_command() {
			return ( new create_rectangle(re,ws) );
		}
		
		//void register_command() {
		//	
		//}
	
		//get_active_command() {
		//	
		//}
		
	private:
		//std::map<>;
		
		runtime_environment* re;
		working_set* ws;
		
};


class renderer
{
	//Q_OBJECT
	public:
		//WPainter(QPaintDevice* p):QPainter(p){
		renderer (QWidget* w) { 	
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
		
		void incr_zoom_factor() {
				m_zoom_factor++;
		}
		
		void decr_zoom_factor() {
				m_zoom_factor--;
		}
		
		//void pan(int x, int y) {
		//	
		//}
		
		QPainter*  get_painter() {
				QRect rect(QPoint(0, 0), QSize(1000,1000));
				QBrush b(Qt::black);
				painter->setBrush(b);
				painter->drawRect(rect);
				
			return painter;
		}
		
	private:
		int m_zoom_factor;
		
	private:
		QPainter* painter;
		QWidget* parent_windget;
};



#endif
