#include "command_manager.hpp"
#include "command.hpp"

command_manager* command_manager::m_instance = 0;


void command_manager::init2(runtime_environment* r, working_set* s) {
    re = {r};
    ws = {s};
    m_current_command = {0};
    m_idle_command = new command_idle();
}

//FIMXE should be called from outside
void command_manager::init() {
    //register_command("create_rect",new create_rectangle_command(re,ws));
    //register_command("idle", new idle_command());
    
    
    //m_rect_command = new command_create_shape<RECT>(re,ws);
    //m_elipse_command = new command_create_shape<ELLIPSE>(re,ws);
    //m_line_command = new command_create_shape<LINE>(re,ws);
    //m_polygon_command = new command_create_shape<POLYGON>(re,ws);
    
    m_current_command = m_idle_command;
}

//icommand_base* get_create_rectangle_command() {
//	return 
//}

icommand_base* command_manager::invoke_command() {
    m_current_command = m_polygon_command;
    return m_current_command;
}

void command_manager::activate_command(icommand_base* cmd) {
    m_current_command = cmd;
}
//icommand_base* get_command() {
//	return 0;
//}

//void command_manager::register_command(const char* nm, icommand_base* cmd) {
//    m_name2command[nm] = cmd;
//}

icommand_base* command_manager::get_active_command() {
    return m_current_command;
}

bool command_manager::is_idle() {
    return ( m_current_command == m_idle_command );
}

void command_manager::return_to_idle() {
    std::cout << "(cm) back to idle" << std::endl;
    m_current_command = m_idle_command;
}



