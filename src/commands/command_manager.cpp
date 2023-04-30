#include "command_manager.hpp"
#include "basic_commands.hpp"
#include "canvas_commands.hpp"
#include "copy_move_commands.hpp"
#include "direct_command_base.hpp"
#include "gui_commands.hpp"
#include "icommand_base.hpp"
#include "qa_commands.hpp"
#include "selection_commands.hpp"

#include "../core/application.hpp"
#include "../core/callback.hpp"
#include "../core/postman.hpp"
#include "../core/renderer.hpp"

#include "../gui/main_window.hpp"
#include "../gui/statusbar_manager.hpp"

#include "../io/messenger.hpp"

#include <cassert>

/// FIXME ????
// int dicmdQaCanvasCompare::n_index = 0;

void command_manager::init2(RuntimePoolManagerPtr r, ObjectPoolPtr s)
{
    // r = re;
    re = r;
    ws = {s};
    m_current_command = nullptr;
    m_idle_command = new incmdIdle();

    REGISTER_CALLBACK(CANVAS_VIEWPORT_CHANGED, &command_manager::on_viewport_changed);
}

// FIMXE should be called from outside
void command_manager::init()
{
    register_command(new dicmdCanvasMouseMove);
    register_command(new dicmdCanvasMouseClick);
    register_command(new dicmdCanvasMouseRelease);
    register_command(new dicmdCanvasMouseDblClick);
    register_command(new dicmdCanvasMousePress);
    register_command(new dicmdguiSelectRadioButton);
    register_command(new dicmdAbortActiveCommand);
    register_command(new dicmdguiSelectComboValue);
    register_command(new dicmdQaToolExit);
    register_command(new dicmdTestCmdListOptions);
    register_command(new dicmdQaCompare<CANVAS>);
    register_command(new dicmdQaCompare<DESIGN>);
    register_command(new dicmdQaCompare<RUNTIME>);
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
    register_command(new dicmdQaReplyingBreak);
    register_command(new dicmdQaReplyingResume);
    register_command(new dicmdQaReplyStep);

    m_current_command = m_idle_command;
}

void command_manager::set_idle_command(CommandBase *cmd)
{
    m_idle_command = cmd;
    m_current_command = m_idle_command;
    activate_command(m_current_command, false);
    // std::cout << "idle is: " <<  m_idle_command << std::endl;
}

CommandBase *command_manager::find_command(const std::string &cmd_name)
{
    // FIXME if non , put error and return idle_command
    // std::cout << "FindCmd: "<< cmd_name << " "<< m_name2command[cmd_name] << std::endl;
    return m_name2command[cmd_name];
}

void command_manager::register_command(CommandBase *cmd)
{
    // FIXME check is not 0
    m_name2command[cmd->get_name()] = cmd;
    // std::cout << "RegCmd: " << cmd->get_name() << "---" << m_name2command[cmd->get_name()]  << std::endl;
}

void command_manager::activate_command(CommandBase *cmd, bool needlog)
{
    // FIXME crashes obviously
    // delete m_current_command;
    std::cout << "activating1: " << m_current_command->is_completed() << std::endl;

    if (cmd == nullptr ||
        (cmd != nullptr && cmd != find_command("dicmdAbortActiveCommand") && !m_current_command->is_completed()))
    {
        Messenger::expose_msg(warn, "please complete/abort current command before activating " + cmd->get_name());
        return;
    }

    std::cout << "activating2: " << cmd << std::endl;

    if (!is_idle() && cmd->get_type() == Interactive)
        m_current_command->abort();

    m_current_command = cmd;

    // m_current_command->activate();

    // if ( m_current_command->get_type == Interactive )
    //         not dummy
    // else
    //         dummy
    if (needlog)
        m_current_command->execute_and_log();
    else
        m_current_command->execute();
}

CommandBase *command_manager::get_active_command()
{
    return m_current_command;
}

bool command_manager::is_idle()
{
    return false;
    return (m_current_command == m_idle_command);
}

void command_manager::disactivate_active_command()
{
    if (!is_idle() && m_current_command->get_type() == Interactive)
    {
        // fixme , abort should be logged implicitly from interactive command base
        dicmdAbortActiveCommand().log();
        m_current_command->abort();
    }

    return_to_idle();
}

void command_manager::return_to_idle()
{
    // std::cout << "(cm) back to idle" << std::endl;
    // delete m_last_command;
    StatusBarManager::getInstance().updateStatusBar("cmIdle.", 1, 0);
    m_current_command = m_idle_command;
    dynamic_cast<main_window *>(m_main_widget)->onCommandDiscard();
    m_current_command->execute();
    m_main_widget->update();
}

// FIXME by keeping wrapper to function
//  when invoking check if m_current_command type is interactive
//  otherwise put wrapper to dummy
/*
void command_manager::dummy(int x, int y) {
}

void command_manager::event_wrapper() {

}
*/

void command_manager::set_main_renderer(renderer *r)
{
    m_renderer = r;
}

renderer *command_manager::get_main_renderer()
{
    return m_renderer;
}

void command_manager::on_viewport_changed(LeCallbackData &d)
{
    canvasTransformClbkDt &d1 = dynamic_cast<canvasTransformClbkDt &>(d);
    m_kx = d1.m_kx;
    m_dx = d1.m_dx;
    m_ky = d1.m_ky;
    m_dy = d1.m_dy;

    /*
    std::cout << "&&&\n";
    std::cout << m_kx << " " << m_ky << std::endl;
    std::cout << m_dx << " " << m_dy << std::endl;
    std::cout << "***\n";
    */
}

void command_manager::mouse_dbl_clicked(int x, int y)
{
    // std::cout << x << "(" << x/m_kx-m_dx << ")  --- " << y << "(" << y/m_ky-m_dy << ")" << std::endl;
    dicmdCanvasMouseDblClick(QPoint(x, y)).log();
    m_current_command->handle_mouse_dblclick(x / m_kx - m_dx, y / m_ky - m_dy);
}

void command_manager::mouse_clicked(int x, int y)
{
   assert(0);
   is_mouse_released = true;
    std::cout << "current commdn is: " << m_current_command << std::endl;
    // std::cout << x << "(" << x/m_kx-m_dx << ")  --- " << y << "(" << y/m_ky-m_dy << ")" << std::endl;
    // find_command("dicmdCanvasMouseClick").log();
    //if (!Application::is_replay_mode())
        //dicmdCanvasMouseClick(QPoint(x, y)).log();

    m_current_command->handle_mouse_click(x / m_kx - m_dx, y / m_ky - m_dy);
}

void command_manager::mouse_moved(int x, int y)
{
    // std::cout << "***current commdn is: " <<  m_current_command << std::endl;
    //dicmdCanvasMouseMove(QPoint(x,y)).log();
    // std::cout << "---mousemove and " << is_mouse_released << std::endl;
    m_current_command->handle_mouse_move(x / m_kx - m_dx, y / m_ky - m_dy, is_mouse_released);
}

void command_manager::mouse_released(int x, int y)
{
    is_mouse_released = true;
    auto command = dynamic_cast<InteractiveCommandBase *>(m_current_command);
    if (command == nullptr)
        return;
    //if (!Application::is_replay_mode())
     //   dicmdCanvasMouseRelease(QPoint(x, y)).log();
    std::cout << "ELRELRLERLELREL: current commdn is: " << m_current_command << std::endl;
    m_current_command->handle_mouse_release(x / m_kx - m_dx, y / m_ky - m_dy);
}

void command_manager::mouse_pressed(int x, int y)
{
    //if (!Application::is_replay_mode())
    //    dicmdCanvasMousePress(QPoint(x, y)).log();

    is_mouse_released = false;
    m_current_command->handle_mouse_press(x / m_kx - m_dx, y / m_ky - m_dy);
}

void command_manager::mouse_pressed2(int x, int y)
{
    // if (!Application::is_replay_mode())
    //	  dicmdCanvasMousePress(QPoint(x, y)).log();
    is_mouse_released = false;
    m_current_command->handle_mouse_press(x / m_kx - m_dx, y / m_ky - m_dy);
}

// FIXME interface?
void command_manager::key_pressed()
{
    m_current_command->handle_key_press();
}

// FIXME interface?
void command_manager::update_tookplace()
{
    m_current_command->handle_update();
}

void command_manager::set_main_widget(QWidget *w)
{
    m_main_widget = w;
}

QWidget *command_manager::get_main_widget()
{
    return m_main_widget;
}
