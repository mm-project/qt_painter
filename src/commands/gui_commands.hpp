#ifndef gui_commands_hpp
#define gui_commands_hpp

#include "command_manager.hpp"
#include "direct_command_base.hpp"

#include <QApplication>
#include <QWidget>
#include <QMouseEvent>
#include <QRadioButton>
#include <QComboBox>
#include <QString>

class dicmdguiSelectComboValue: public DirectCommandBase 
{

    std::string m_on;
    std::string m_ov;
    public:
        dicmdguiSelectComboValue() {
            add_option("-object",new StringCommandOptionValue(""));
            add_option("-value",new StringCommandOptionValue(""));
        }
        
        dicmdguiSelectComboValue(const std::string& on, const std::string& ov)//:DirectCommandBase("-object",new StringCommandOptionValue(on)) 
        { 
            //FIXME add_option()
            m_on = on;
            m_ov = ov;
            
            add_option("-object",new StringCommandOptionValue(m_on));
            add_option("-value",new StringCommandOptionValue(m_ov));
        }
        
        virtual std::string get_name() {
            return "dicmdguiSelectComboValue";
        }
        
        virtual void execute() {
            /*//FIXME add checks
            //FIXME some trick to be more easy?
            m_on = GET_CMD_ARG(StringCommandOptionValue,"-object");
            m_ov = GET_CMD_ARG(StringCommandOptionValue,"-value");
            QString s(m_ov.c_str());
            s.replace("/"," ");   
            QComboBox* cmb = command_manager::get_instance()->get_main_widget()->findChild<QComboBox*>(m_on.c_str());
            
            std::cout << cmb->currentText().toStdString() << " " << m_on << " " << cmb->findData("White") << std::endl;
            cmb->setCurrentIndex(cmb->findData("White"));
            //btn->click();*/
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
            //FIXME add checks
            //FIXME some trick to be more easy?
            m_on = GET_CMD_ARG(StringCommandOptionValue,"-object");
            //m_on = (dynamic_cast<StringCommandOptionValue*>(get_option_val("-object")))->to_string();
            QRadioButton* btn = command_manager::get_instance()->get_main_widget()->findChild<QRadioButton*>(m_on.c_str());
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
            m_p = GET_CMD_ARG(PointCommandOptionValue,"-point");
            //m_p = (dynamic_cast<PointCommandOptionValue*>(get_option_val("-point")))->get();
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
            //m_p = (dynamic_cast<PointCommandOptionValue*>(get_option_val("-point")))->get();
            m_p = GET_CMD_ARG(PointCommandOptionValue,"-point");
            dicmdCanvasMouseMove(m_p).execute();
            QMouseEvent event(QEvent::MouseButtonPress, m_p, Qt::LeftButton, 0, 0);
            QApplication::sendEvent(command_manager::get_instance()->get_main_widget()->findChild<QWidget*>("CANVAS"), &event);
        } 
};



#endif
