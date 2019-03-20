#ifndef basic_commands_hpp
#define basic_commands_hpp

#include "command_manager.hpp"
#include "interactive_command_base.hpp"
#include "direct_command_base.hpp"

#include <QApplication>
#include <QWidget>
#include <QMouseEvent>

class incmdIdle : public DirectCommandBase
{
    public:
        virtual void execute() {} 
        virtual std::string get_name() { "incmdIdle"; }
};


class dicmdAbortActiveCommand: public DirectCommandBase
{
    public:        
        virtual std::string get_name() {
            return "dicmdAbortActiveCommand";
        }

        virtual void execute() {
            command_manager::get_instance()->get_active_command()->abort();
        }

};

class dicmdguiSelectRadioButton: public DirectCommandBase 
{

    std::string m_on;
    public:
        dicmdguiSelectRadioButton() {
        }
        
        dicmdguiSelectRadioButton(const std::string& on):DirectCommandBase("-object",new StringCommandOptionValue(on)) 
        { m_on = on; }
        
        virtual std::string get_name() {
            return "dicmdguiSelectRadioButton";
        }
        
        virtual void execute() {
            //check option added
            //qApp->findChildren<QWidget*>(m_on)->setSelected(true);
        }
};

class dicmdCanvasAddPoint: public DirectCommandBase
{
    
    QPoint m_p;
    public:

        dicmdCanvasAddPoint() {
        }
        
        dicmdCanvasAddPoint(const QPoint& p):DirectCommandBase("-point",new PointCommandOptionValue(p)) 
        { m_p = p; }
        
        virtual std::string get_name() {
            return "dicmdCanvasAddPoint";
        }
        
        virtual void execute() {
            //QList<QWidget*> ws = qApp->findChildren<QWidget*>();
            //std::cout << ws.size() << std::endl;
            //assert(0);
            //QMouseEvent event(QEvent::MouseButtonPress, point(), Qt::LeftButton, 0, 0);
            //QApplication::sendEvent(w, &event);
            //}
        } 

                
        
        //FIXME make generic
        //FIXME check dynamic_cast before *
        //QPoint point() {
        //    return *(dynamic_cast<PointCommandOption*>(get_arg()));
        //}
};


#endif
