#ifndef basic_commands_hpp
#define basic_commands_hpp

#include "command_manager.hpp"
#include "interactive_command_base.hpp"
#include "direct_command_base.hpp"

#include <QApplication>
#include <QWidget>
#include <QMouseEvent>

class incmdIdle : public InteractiveCommandBase
{
    public:
        virtual void execute() {} 
        virtual void abort() {}
        virtual void log(const std::string& msg) {}
        virtual std::string get_name() { "incmdIdle"; }
};


class dicmdAbortActiveCommand: public DirectCommandBase
{
    public:
        virtual void execute() {
            command_manager::get_instance()->get_active_command()->abort();
        }
        
        virtual std::string get_name() {
            return "dicmdAbortActiveCommand";
        }
    
        virtual void abort() {}

};

class dicmdCanvasAddPoint: public DirectCommandBase
{
    bool m_is_dummy;
    
    public:
        dicmdCanvasAddPoint(bool dummy = false ):m_is_dummy(dummy) {
        }
        
        virtual void execute() {
            if ( ! m_is_dummy ) {
                QList<QWidget*> ws = qApp->findChildren<QWidget*>();
                //std::cout << ws.size() << std::endl;
                //assert(0);
                //QMouseEvent event(QEvent::MouseButtonPress, point(), Qt::LeftButton, 0, 0);
                //QApplication::sendEvent(w, &event);
            }
        } 
        
        virtual void log(const std::string&) {
            std::cout << get_name() << " " << point().x() << " " << point().y() << std::endl;
        }
        
        virtual std::string get_name() {
            return "dicmdCanvasAddPoint";
        }
        
        virtual void abort() {
        }
        
        //FIXME make generic
        //FIXME check dynamic_cast before *
        QPoint point() {
            return *(dynamic_cast<PointCommandOption*>(get_arg()));
        }
};


#endif
