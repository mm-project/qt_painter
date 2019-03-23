#ifndef basic_commands_hpp
#define basic_commands_hpp

#include "command_manager.hpp"
#include "interactive_command_base.hpp"
#include "direct_command_base.hpp"

#include <QApplication>
#include <QWidget>
#include <QMouseEvent>
#include <QRadioButton>

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
            add_option("-object",new StringCommandOptionValue(""));
        }
        
        dicmdguiSelectRadioButton(const std::string& on):DirectCommandBase("-object",new StringCommandOptionValue(on)) 
        { 
            //FIXME add_option()
            m_on = on; 
        }
        
        virtual std::string get_name() {
            return "dicmdguiSelectRadioButton";
        }
        
        virtual void execute() {
            //FIXME some trick to be more easy?
            m_on = (dynamic_cast<StringCommandOptionValue*>(get_option_val("-object")))->to_string();
            QRadioButton* btn = command_manager::get_instance()->get_main_widget()->findChild<QRadioButton*>(m_on.c_str());
            //btn->setChecked(true);
            //btn->setEnabled(false);
            //btn->setVisible(false);    
            btn->click();
        }
};


class dicmdCanvasMouseMove: public DirectCommandBase
{
    
    QPoint m_p;
    public:

        dicmdCanvasMouseMove() {
            add_option("-point",new PointCommandOptionValue(QPoint(0,0)));
            //FIXME add not initizialize flag
        }
        
        dicmdCanvasMouseMove(const QPoint& p):DirectCommandBase("-point",new PointCommandOptionValue(p)) 
        { m_p = p; }
        
        virtual std::string get_name() {
            return "dicmdCanvasMouseMove";
        }

        //FIXME check dynamic_cast before *
        virtual void execute() {
            m_p = (dynamic_cast<PointCommandOptionValue*>(get_option_val("-point")))->get();
            QMouseEvent event(QEvent::MouseMove, m_p, Qt::LeftButton, 0, 0);
            QApplication::sendEvent(command_manager::get_instance()->get_main_widget()->findChild<QWidget*>("CANVAS"), &event);
        } 
};


class dicmdCanvasMouseClick: public DirectCommandBase
{
    
    QPoint m_p;
    public:

        dicmdCanvasMouseClick() {
            add_option("-point",new PointCommandOptionValue(QPoint(0,0)));
        }
        
        dicmdCanvasMouseClick(const QPoint& p):DirectCommandBase("-point",new PointCommandOptionValue(p)) 
        { m_p = p; }
        
        virtual std::string get_name() {
            return "dicmdCanvasMouseClick";
        }
        
        virtual void execute() {
            m_p = (dynamic_cast<PointCommandOptionValue*>(get_option_val("-point")))->get();
            dicmdCanvasMouseMove(m_p).execute();
            QMouseEvent event(QEvent::MouseButtonPress, m_p, Qt::LeftButton, 0, 0);
            QApplication::sendEvent(command_manager::get_instance()->get_main_widget()->findChild<QWidget*>("CANVAS"), &event);
        } 
};



#endif