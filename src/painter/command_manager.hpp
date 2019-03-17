#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "icommand_base.hpp"
#include "command.hpp"
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
        
        void activate_command(icommand_base* cmd);
        icommand_base* invoke_command();
        //icommand_base* get_command();
        //void register_command(const char* nm, icommand_base* cmd) 
        icommand_base* get_active_command();
        bool is_idle(); 
        void return_to_idle(); 
        
    private:
        //FIXME !!! map with string
        std::map<std::string, icommand_base*> m_name2command;
        
        icommand_base* m_current_command;
        icommand_base* m_idle_command;
 
        icommand_base* m_elipse_command;
        icommand_base* m_line_command;
        icommand_base* m_rect_command;
        icommand_base* m_polygon_command;

        
        runtime_environment* re;
        working_set* ws;
        //bool m_is_idle;

    public:
        static command_manager* m_instance;
};




#endif

