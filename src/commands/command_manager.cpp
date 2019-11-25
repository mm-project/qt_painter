#include "command_manager.hpp"
#include "icommand_base.hpp"
#include "basic_commands.hpp"
#include "direct_command_base.hpp"
#include "basic_commands.hpp"
#include "gui_commands.hpp"
#include "shape_creator_commands.hpp"
#include "selection_commands.hpp"


#include <cassert>

command_manager* command_manager::m_instance = 0;


void command_manager::init2(ObjectPoolSandboxPtr r, IObjectPoolPtr s) {
	r = re;
    ws = {s};
    m_current_command = {0};
    m_idle_command = new incmdIdle();
}

//FIMXE should be called from outside
void command_manager::init() {
    register_command(new dicmdCanvasMouseMove);
    register_command(new dicmdCanvasMouseClick);
    register_command(new dicmdCanvasMouseDblClick);
    register_command(new dicmdguiSelectRadioButton);
    register_command(new dicmdAbortActiveCommand);
    register_command(new dicmdguiSelectComboValue); 
    //register_command(new incmdSelectShapesByRegion);
    
    m_current_command = m_idle_command;
}


CommandBase* command_manager::find_command(const std::string& cmd_name) {
    //FIXME if non , put error and return idle_command
    //std::cout << "FindCmd: "<< cmd_name << " "<< m_name2command[cmd_name] << std::endl;
    return m_name2command[cmd_name];
}

void command_manager::register_command(CommandBase* cmd) {
    //FIXME check is not 0
    m_name2command[cmd->get_name()] = cmd;
    //std::cout << "RegCmd: " << cmd->get_name() << "---" << m_name2command[cmd->get_name()]  << std::endl;
}

void command_manager::activate_command(CommandBase* cmd) {
    //FIXME crashes obviously
    //delete m_current_command;
    
    if ( !is_idle() && cmd->get_type() == Interactive )
		m_current_command->abort(); 
    
    m_current_command = cmd;
    //m_current_command->activate();
    
    //if ( m_current_command->get_type == Interactive )
    //        not dummy
    //else
    //        dummy
            
    m_current_command->execute_and_log();
}

CommandBase* command_manager::get_active_command() {
    return m_current_command;
}

bool command_manager::is_idle() {
    return ( m_current_command == m_idle_command );
}

void command_manager::return_to_idle() {
    //std::cout << "(cm) back to idle" << std::endl;
    //delete m_last_command;
    m_current_command = m_idle_command;
}

//FIXME by keeping wrapper to function 
// when invoking check if m_current_command type is interactive
// otherwise put wrapper to dummy
/*
void command_manager::dummy(int x, int y) {
}

void command_manager::event_wrapper() {
    
}
*/

void command_manager::mouse_dbl_clicked(int x, int y) {
    m_current_command->handle_mouse_dblclick(x,y);
}

void command_manager::mouse_clicked(int x, int y) {
    m_current_command->handle_mouse_click(x,y);
}

void command_manager::mouse_moved(int x, int y) {
     m_current_command->handle_mouse_move(x,y);
}

//FIXME interface?
void command_manager::key_pressed() {
    m_current_command->handle_key_press();
}

//FIXME interface?
void command_manager::update() {
     m_current_command->handle_update();
}

void command_manager::set_main_widget(QWidget* w) {
    m_main_widget = w;
}

QWidget* command_manager::get_main_widget() {
    return m_main_widget;
}
