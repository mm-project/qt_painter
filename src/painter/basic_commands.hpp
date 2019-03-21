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
        { m_on = on; }
        
        virtual std::string get_name() {
            return "dicmdguiSelectRadioButton";
        }
        
        virtual void execute() {
            //check option added
            //qApp->findChildren<QWidget*>(m_on)->setSelected(true);
            //QMouseEvent event(QEvent::MouseButtonPress, m_p, Qt::LeftButton, 0, 0);
            //QApplication::sendEvent(command_manager::get_instance()->get_main_widget()->findChild<QWidget*>(m_on), &event);
            QWidget* btn = command_manager::get_instance()->get_main_widget()->findChild<QWidget*>(m_on.c_str());
            std::cout << "-------------------------" << btn << std::endl;
            
            //btn->setChecked(true);
            //btn->setEnabled(false);
            //btn->setVisible(false);    
            //btn->click();
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
            //QList<QWidget*> ws = qApp->findChild<QWidget*>("CANVAS");
            //std::cout << ws.size() << std::endl;
            //assert(0);
            m_p = (dynamic_cast<PointCommandOptionValue*>(get_option_val("-point")))->get();
            QMouseEvent event(QEvent::MouseButtonPress, m_p, Qt::LeftButton, 0, 0);
            QApplication::sendEvent(command_manager::get_instance()->get_main_widget()->findChild<QWidget*>("CANVAS"), &event);
        } 

        //FIXME make generic
        //FIXME check dynamic_cast before *
        //QPoint point() {
        //    return *(dynamic_cast<PointCommandOption*>(get_arg()));
        //}
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
        
        virtual void execute() {
            //QList<QWidget*> ws = qApp->findChild<QWidget*>("CANVAS");
            //std::cout << ws.size() << std::endl;
            //assert(0);
            m_p = (dynamic_cast<PointCommandOptionValue*>(get_option_val("-point")))->get();
            QMouseEvent event(QEvent::MouseMove, m_p, Qt::LeftButton, 0, 0);
            QApplication::sendEvent(command_manager::get_instance()->get_main_widget()->findChild<QWidget*>("CANVAS"), &event);
        } 

        //FIXME make generic
        //FIXME check dynamic_cast before *
        //QPoint point() {
        //    return *(dynamic_cast<PointCommandOption*>(get_arg()));
        //}
};


#endif
