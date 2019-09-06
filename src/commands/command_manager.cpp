#include "command_manager.hpp"
#include "icommand_base.hpp"
#include "basic_commands.hpp"
#include "direct_command_base.hpp"
#include "basic_commands.hpp"
#include "gui_commands.hpp"
#include "selection_commands.hpp"
#include "qa_commands.hpp"
#include "canvas_commands.hpp"
#include "copy_move_commands.hpp"

#include "../core/postman.hpp"
#include "../core/callback.hpp"
#include "../core/renderer.hpp"

#include "../gui/statusbar_manager.hpp"
#include "../gui/main_window.hpp"


#include <cassert>

command_manager* command_manager::m_instance = 0;
///FIXME ????
//int dicmdQaCanvasCompare::n_index = 0;

void command_manager::init2(ObjectPoolSandboxPtr r, IObjectPoolPtr s) {
	r = re;
    ws = {s};
    m_current_command = {0};
    m_idle_command = new incmdIdle();
    
    REGISTER_CALLBACK(CANVAS_VIEWPORT_CHANGED,&command_manager::on_viewport_changed);
}

//FIMXE should be called from outside
void command_manager::init() {
    register_command(new dicmdCanvasMouseMove);
    register_command(new dicmdCanvasMouseClick);
    register_command(new dicmdCanvasMouseDblClick);
    register_command(new dicmdguiSelectRadioButton);
    register_command(new dicmdAbortActiveCommand);
    register_command(new dicmdguiSelectComboValue); 
    register_command(new dicmdQaToolExit);
    register_command(new dicmdTestCmdListOptions);
    register_command(new dicmdQaCompare<CANVAS>);
    register_command(new dicmdQaCompare<DESIGN>);
    register_command(new dicmdQaCompare<SELECTION>);
    register_command(new dicmdSelectShapesByRegion);
    register_command(new dicmdguiClickButton);
    register_command(new dicmdguiClickModalButton);
    register_command(new dicmdCanvasOrigin<PANDOWN>);
    register_command(new dicmdCanvasOrigin<PANLEFT>);
    register_command(new dicmdCanvasOrigin<PANRIGHT>);
    register_command(new dicmdCanvasOrigin<PANUP>);
    register_command(new dicmdCanvasViewport<ZOOMIN>);
    register_command(new dicmdCanvasViewport<ZOOMOUT>);

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


void command_manager::disactivate_active_command() {
    if ( !is_idle() && m_current_command->get_type() == Interactive ) {
            //fixme , abort should be logged implicitly from interactive command base
            dicmdAbortActiveCommand().log();
            m_current_command->abort(); 
    }
    
    return_to_idle();
}

void command_manager::return_to_idle() {
    //std::cout << "(cm) back to idle" << std::endl;
    //delete m_last_command;
    StatusBarManager::getInstance().updateStatusBar("Idle.",1,0);
    m_current_command = m_idle_command;
    dynamic_cast<main_window*>(m_main_widget)->onCommandDiscard();
    m_main_widget->update();
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


void command_manager::set_main_renderer(renderer* r) 
{
    m_renderer = r;
}

renderer* command_manager::get_main_renderer()
{
    return m_renderer;
}

void command_manager::on_viewport_changed(LeCallbackData& d)
{
    canvasTransformClbkDt& d1 = dynamic_cast<canvasTransformClbkDt&>(d);
    m_kx = d1.m_kx;
    m_dx = d1.m_dx;
    m_ky = d1.m_ky;
    m_dy = d1.m_dy;
    
    /*
    std::cout << "&&&\n";
    std::cout << m_kx << " " << m_ky << std::endl;
    std::cout << m_dx << " " << m_dy << std::endl;
    std::cout << "***\n";
    /**/
   
    
}

void command_manager::mouse_dbl_clicked(int x, int y) {
    //std::cout << x << "(" << x/m_kx-m_dx << ")  --- " << y << "(" << y/m_ky-m_dy << ")" << std::endl;  

    m_current_command->handle_mouse_dblclick(x/m_kx-m_dx,y/m_ky-m_dy);
}

void command_manager::mouse_clicked(int x, int y) {
    //std::cout << x << "(" << x/m_kx-m_dx << ")  --- " << y << "(" << y/m_ky-m_dy << ")" << std::endl;  
    m_current_command->handle_mouse_click(x/m_kx-m_dx,y/m_ky-m_dy);
}

void command_manager::mouse_moved(int x, int y) {
    m_current_command->handle_mouse_move(x/m_kx-m_dx,y/m_ky-m_dy);
}

//FIXME interface?
void command_manager::key_pressed() {
    m_current_command->handle_key_press();
}

//FIXME interface?
void command_manager::update_tookplace() {
     m_current_command->handle_update();
}

void command_manager::set_main_widget(QWidget* w) {
    m_main_widget = w;
}

QWidget* command_manager::get_main_widget() {
    return m_main_widget;
}
