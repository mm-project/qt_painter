#ifndef interactive_command_base
#define interactive_command_base

#include "icommand_base.hpp"

#include "../core/application.hpp"
#include "../core/event.hpp"

#include <functional>
#include <iostream>
#include <string>

#define HANDLE_FUNCTION(C, M) std::bind(&C::M, this, std::placeholders::_1)
typedef std::function<void(const EvType &)> CmdMemFun;

class InteractiveCommandBase : public CommandBase
{

    bool m_is_released = true;

  public:
    virtual CommandType get_type()
    {
        return Interactive;
    }

    virtual void handle_mouse_release(int x, int y)
    {
        // log("click "+x+" "+y);
        //if (!m_is_released)
        //    return;

        m_last_release_point.setX(x);
        m_last_release_point.setY(y);
        m_current_event_handler(MU);
        m_is_released = true;
        std::cout << "------------------------------------------------------------RELEASE\n";
    }

    virtual void handle_mouse_press(int x, int y)
    {
		m_last_press_point.setX(x);
        m_last_press_point.setY(y);
        m_current_event_handler(MD);
        m_is_released = false;
        std::cout << "------------------------------------------------------------PRESSED\n";
    }

    virtual void handle_mouse_click(int x, int y)
    {
        // log("click "+x+" "+y);
        m_last_click_point.setX(x);
        m_last_click_point.setY(y);
        m_current_event_handler(MC);
        m_is_released = true;

        /*
        if(Application::is_replay_mode()) {
            m_current_event_handler(MC);
            m_is_released = true;
            std::cout << "------------------------------------------------------------CLICKED2\n";
            return;
        }
        */
		/*
        if (m_is_released)
        {
            m_is_released = false;
            m_current_event_handler(MU);
            std::cout << "------------------------------------------------------------RELEASED\n";
        }
        else
        {
            std::cout << "------------------------------------------------------------HOLD\n";
            m_current_event_handler(MD);
            m_is_released = true;
        }
		*/
    }

    virtual void handle_mouse_move(int x, int y, bool is_released)
    {
        m_last_cursor_point.setX(x);
        m_last_cursor_point.setY(y);
		//std::cout << "------------------------------------------------------------MOVE " << is_released << std::endl;
        
		//assert(is_released == true);
		//std::cout << "------------------------------------------------------------MOVE " << is_released << std::endl;
        if  (is_released)
			m_current_event_handler(MM);
		else
			m_current_event_handler(MDM);			
    }

    virtual void handle_mouse_dblclick(int, int)
    {
        // log("dblclick "+x+" "+y);
        m_current_event_handler(MDC);
    }

    virtual void handle_key_press()
    {
        // log("dblclick "+x+" "+y);
        m_current_event_handler(KP);
    }

    virtual bool need_log_mouserelease()
    {
        return false;
    }

  public:
    void set_auto_repeat(bool m)
    {
        m_autorepeat = m;
    }

    bool is_auto_repeating()
    {
        return m_autorepeat;
    }

    void set_next_handler(CmdMemFun fun)
    {
        m_current_event_handler = fun;
    }

    QPoint get_last_point()
    {
        return m_last_cursor_point;
    }

    QPoint get_lastclk_point()
    {
        return m_last_click_point;
    }

    QPoint get_lastprs_point()
    {
        return m_last_press_point;
    }

  private:
    CmdMemFun m_current_event_handler;
    QPoint m_last_cursor_point;
    QPoint m_last_click_point;
    QPoint m_last_release_point;
    QPoint m_last_press_point;

    bool m_autorepeat = true;
};

#endif
