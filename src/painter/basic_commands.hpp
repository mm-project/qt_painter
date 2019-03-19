#ifndef basic_commands_hpp
#define basic_commands_hpp

#include "interactive_command_base.hpp"
#include "direct_command_base.hpp"

#include <QApplication>
#include <QWidget>
#include <QMouseEvent>

class incmd_idle : public interactive_command_base
{
    public:
        virtual void execute_and_log() {} 
        virtual void process() {} 
        virtual void mouse_clicked(int, int) {}
        virtual void mouse_moved(int, int) {} 
        virtual void mouse_dbl_clicked(int, int) {};
        virtual void log() {}
        virtual void update() {}
};


class dicmd_add_point_to_canvas: public direct_command_base
{
    public:
        virtual void execute() {
            QWidget* w = qApp()->findChild<QWidget*>("CANVAS")
            QMouseEvent event(QEvent::MouseButtonPress, get_arg(), 0, 0, 0);
            QApplication::sendEvent(w, &event)
        } 
};


#endif
