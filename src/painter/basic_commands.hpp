#ifndef basic_commands_hpp
#define basic_commands_hpp


#include "interactive_command_base.hpp"
#include "direct_command_base.hpp"

#include <QApplication>
#include <QWidget>
#include <QMouseEvent>

class incmdIdle : public InteractiveCommandBase
{
    public:
        virtual void execute() {} 
        virtual void log() {}
        virtual std::string get_name() { "incmdIdle"; }
};


class dicmdCanvasAddPoint: public DirectCommandBase
{
    public:
        virtual void execute() {
            QWidget* w = qApp->findChild<QWidget*>("CANVAS");
            QMouseEvent event(QEvent::MouseButtonPress, QPointF(0,0), Qt::LeftButton, 0, 0);
            //QMouseEvent event(QEvent::MouseButtonPress, (*get_arg())(QPoint), 0, 0, 0);
            QApplication::sendEvent(w, &event);
        } 
        
        virtual void log() {
            //std::cout << get_name() << " " << get_arg().x() << " " << get_arg().y() << std::endl;
        }
        
        virtual std::string get_name() {
            return "dicmdCanvasAddPoint";
        }
};


#endif
