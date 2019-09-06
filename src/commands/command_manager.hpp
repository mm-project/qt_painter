#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "icommand_base.hpp"

#include "../core/runtime_environment.hpp"
#include "../core/working_set.hpp"
#include "../core/callback.hpp"


// Stl
#include <string>
#include <iostream>

// Qt
#include <QWidget>
 
///////////////////////////////////////////////////////////////////////////////
//
// @class command manager
//
class renderer;
class command_manager  
{

    public:
        static command_manager* get_instance() {
            if ( ! m_instance ) 
                m_instance = new command_manager;
            return m_instance;
        }
        
    public:
        void set_main_widget(QWidget* w);
        QWidget* get_main_widget();
        void set_main_renderer(renderer* r);
        renderer* get_main_renderer();
        
        
        void init2(ObjectPoolSandboxPtr r, IObjectPoolPtr s);
        void init();
        
        void set_idle_command(CommandBase* cmd);
        void disactivate_active_command();
        void activate_command(CommandBase* cmd);
        void register_command(CommandBase* cmd);
        CommandBase* find_command(const std::string&);
        //CommandBase* get_command();
        //void register_command(const char* nm, CommandBase* cmd) 
        CommandBase* get_active_command();
        bool is_idle(); 
        void return_to_idle(); 
        
        void mouse_dbl_clicked(int x, int y);
        void mouse_clicked(int x, int y);
        void mouse_released(int, int);
        void mouse_moved(int x, int y);
        void key_pressed();
        void update_tookplace();
        
        void on_viewport_changed(LeCallbackData& d);

        
    private:
        renderer* m_renderer;
        //FIXME !!! map with string
        std::map<std::string, CommandBase*> m_name2command;
        
        CommandBase* m_current_command = nullptr;
        CommandBase* m_idle_command = nullptr;
 
        //CommandBase* m_elipse_command;
        //CommandBase* m_line_command;
        //CommandBase* m_rect_command;
        //CommandBase* m_polygon_command;

        
        ObjectPoolSandboxPtr re;
        IObjectPoolPtr ws;
        //bool m_is_idle;
        QWidget* m_main_widget	= nullptr;
        float m_kx = 1;
        float m_ky = 1;
        int m_dx = 0;
        int m_dy = 0;

    public:
        static command_manager* m_instance;
};






#endif

