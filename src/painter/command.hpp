#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <QWidget>
#include <QPainter>
#include <QPaintDevice>

#include "controller.hpp"
#include "runtime_environment.hpp"
#include "working_set.hpp"

class command_base
{
	public:
		void virtual execute() {
			//while ( isFinished() ) {
			//	process();
			//}
			//if (LOG_MODE) 
			//	execute();
			//	log();
			//else
			//	execute_and_log();
		}
		
		//bool virtual isFinished() = 0;
		//void virtual execute_and_log() = 0;
		void virtual process() = 0;
		void virtual mouse_clicked(int, int) = 0;
		void virtual mouse_move(int, int) = 0;
		//void virtual log() = 0;
		
	
};

class idle_command : public command_base {
		public:
			void virtual execute_and_log() {} 
			void virtual process() {} 
			void virtual mouse_clicked(int, int) {}
			void virtual mouse_move(int, int) {} 
			void virtual log() {}
	
};



class create_rectangle_command : public command_base
{
	public:
		create_rectangle_command(runtime_environment* r,working_set* s):re(r),ws(s) {
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







#endif
