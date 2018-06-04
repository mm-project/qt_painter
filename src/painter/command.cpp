#include "command.hpp"
#include "command_manager.hpp"

create_rectangle_command::create_rectangle_command(runtime_environment* r,working_set* s):re(r),ws(s) 
{
	m_is_first_click = true;
	m_finished = false;
	re->change_object_type(RECT);
}

bool create_rectangle_command::isFinished() 
{
	return m_finished;
}

void create_rectangle_command::process() 
{
	m_finished = true;
}

void  create_rectangle_command::mouse_clicked(int x, int y) 
{
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
		command_manager::get_instance()->return_to_idle();
		m_is_first_click = true;
	}
}

void  create_rectangle_command::mouse_move(int x, int y) 
{
	if ( m_finished ) return;
	re->set_pos2(QPoint(x,y));
}
