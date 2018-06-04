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
		create_rectangle_command(runtime_environment* r,working_set* s);
		virtual bool isFinished();
		virtual void process();
		virtual void  mouse_clicked(int x, int y) override;
		virtual void  mouse_move(int x, int y);
		
	private:
		bool m_is_first_click;
		bool m_finished;
		runtime_environment* re;
		working_set* ws;
};







#endif
