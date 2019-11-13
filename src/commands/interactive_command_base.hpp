#ifndef interactive_command_base
#define interactive_command_base

#include "icommand_base.hpp"

#include "../core/event.hpp"


#include <string>
#include <functional>

#define HANDLE_FUNCTION(C,M) std::bind(&C::M,this,std::placeholders::_1)
typedef std::function<void( const EvType& )> CmdMemFun;

class InteractiveCommandBase : public CommandBase
{

    bool m_is_released = false;
    public:
        virtual CommandType get_type() {
            return Interactive;
        }

        virtual void handle_mouse_release (int x , int y) {
            //log("click "+x+" "+y);
            m_last_click_point.setX(x);
            m_last_click_point.setY(y);
            m_current_event_handler(MR);
            m_is_released = true;
        }
        
        virtual void handle_mouse_click(int x , int y) {
            //log("click "+x+" "+y);
            m_last_click_point.setX(x);
            m_last_click_point.setY(y);

            if ( m_is_released ) {
                m_is_released = false;
                m_current_event_handler(MC);
            } else {
                m_current_event_handler(MP);
            }
        }
        
        virtual void handle_mouse_move(int x, int y) {
            m_last_cursor_point.setX(x);
            m_last_cursor_point.setY(y);
            m_current_event_handler(MM);
        } 
        
        virtual void handle_mouse_dblclick(int, int) {
            //log("dblclick "+x+" "+y);
            m_current_event_handler(MDC);
        }
       
        virtual void handle_key_press() {
            //log("dblclick "+x+" "+y);
            m_current_event_handler(KP);
        }
       
    public:
        void set_auto_repeat(bool m) {
            m_autorepeat = m;
            
        }
        
        bool is_auto_repeating() {
            return m_autorepeat;
        }
        
        void set_next_handler( CmdMemFun fun ) {
            m_current_event_handler = fun;
        }
        
        QPoint get_last_point() {
            return m_last_cursor_point;
        }

                
        QPoint get_lastclk_point() {
            return m_last_click_point;
        }

        
    private:
        CmdMemFun m_current_event_handler;
        QPoint m_last_cursor_point;
        QPoint m_last_click_point;
        bool m_autorepeat = true;

};


#endif
