#ifndef interactive_command_base
#define interactive_command_base

#include "icommand.hpp"
#include "event.hpp"


#include <string>

#define MEMBER_FUNCTION(C,M) std::bind(&C::M,this,std::placeholders::_1)
typedef std::function<void( const EvType& )> CmdMemFun;

class interactive_command_base : public icommand

    public:
        interactive_command_base(runtime_environment* r,working_set*s):re(r),ws(s) {}
        
    public:

        void virtual log(const std::string&) {
            //if ( ... )
            std::cout << msg << std::endl;
        }
        
        void virtual execute_and_log() {
            log(get_name())
            execute();
        } 
        
        
        void virtual mouse_clicked(int x, inty) {
            log("click "+x+" "+y);
            m_current_event_handler(MC);
        }
        
        void virtual mouse_moved(int x, int y) {
            m_last_cursor_point.setX(x);
            m_last_cursor_point.setY(y);
            m_current_event_handler(MM);
        } 
        
        void virtual mouse_dbl_clicked(int, int) {
            log("dblclick "+x+" "+y);
            m_current_event_handler(MDC);
        }
        
        void virtual update() = 0
        
        void set_next_step( CmdMemFun fun ) {
            m_current_event_handler = fun;
        }
        
        QPoint get_last_point() {
            return m_last_cursor_point;
        }

                
        
    private:
        CmdMemFun m_current_event_handler;
        QPoint m_last_cursor_point;

};


#endif
