#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "icommand_base.hpp"
#include "runtime_environment.hpp"
#include "working_set.hpp"

// Stl
#include <string>
#include <iostream>
 
///////////////////////////////////////////////////////////////////////////////
//
// @class command manager
//
class command_manager  
{

    public:
        static command_manager* get_instance() {
            if ( ! m_instance ) 
                m_instance = new command_manager;
            return m_instance;
        }
        
    public:
        
        void init2(runtime_environment* r, working_set* s);
        void init();
        
        void activate_command(CommandBase* cmd);
        CommandBase* invoke_command();
        //CommandBase* get_command();
        //void register_command(const char* nm, CommandBase* cmd) 
        CommandBase* get_active_command();
        bool is_idle(); 
        void return_to_idle(); 
        
        void mouse_dbl_clicked(int x, int y);
        void mouse_clicked(int x, int y);
        void mouse_moved(int x, int y);
        void key_pressed();
        void update();
        
        
    private:
        //FIXME !!! map with string
        std::map<std::string, CommandBase*> m_name2command;
        
        CommandBase* m_current_command;
        CommandBase* m_idle_command;
 
        //CommandBase* m_elipse_command;
        //CommandBase* m_line_command;
        //CommandBase* m_rect_command;
        //CommandBase* m_polygon_command;

        
        runtime_environment* re;
        working_set* ws;
        //bool m_is_idle;

    public:
        static command_manager* m_instance;
};




#endif

