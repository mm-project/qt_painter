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
#include <QMessageBox>
#include <QPushButton>

class NonTransactionalDirectCommandBase : public DirectCommandBase
{
    public:
         NonTransactionalDirectCommandBase() {}
        NonTransactionalDirectCommandBase(const std::string& n, ICommandOptionValue* v ):DirectCommandBase(n,v) {}
        virtual bool is_transaction_cmd() {
            return false;
        }
    
};

class dicmdguiSelectComboValue: public NonTransactionalDirectCommandBase 
{

    std::string m_on;
    std::string m_ov;
    public:
        dicmdguiSelectComboValue() {
            add_option("-object",new StringCommandOptionValue(""));
            add_option("-value",new StringCommandOptionValue(""));
        }
        
        dicmdguiSelectComboValue(const std::string& on, const std::string& ov)//:NonTransactionalDirectCommandBase("-object",new StringCommandOptionValue(on)) 
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


class dicmdguiPressButton: public NonTransactionalDirectCommandBase 
{

    std::string m_op1;
    std::string m_op2;
    
    public:
        dicmdguiPressButton() {
            add_option("-btn_name",new StringCommandOptionValue(""));
            add_option("-in_window",new StringCommandOptionValue(""));
        }
        
        dicmdguiPressButton(const std::string& op1, const std::string& op2)
        { 
            add_option("-btn_name",new StringCommandOptionValue(op1));
            add_option("-in_window",new StringCommandOptionValue(op2));
        }
        
        virtual std::string get_name() {
            return "dicmdguiPressButton";
        }
        
        virtual void execute() {
            QWidget* w = command_manager::get_instance()->get_main_widget();
            m_op1 = GET_CMD_ARG(StringCommandOptionValue,"-btn_name");
            m_op2 = GET_CMD_ARG(StringCommandOptionValue,"-in_window");
            QMessageBox* box = w->findChild<QMessageBox*>();
            box->buttons().back()->click();
            //QPushButton* btn = box->findChild<QPushButton*>(m_op1.c_str());
            //btn->click();
        }
};


class dicmdguiSelectRadioButton: public NonTransactionalDirectCommandBase 
{

    std::string m_on;
    public:
        dicmdguiSelectRadioButton() {
            add_option("-object",new StringCommandOptionValue(""));
        }
        
        dicmdguiSelectRadioButton(const std::string& on):NonTransactionalDirectCommandBase("-object",new StringCommandOptionValue(on)) 
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


class dicmdCanvasMouseMove: public NonTransactionalDirectCommandBase
{
    
    QPoint m_p;
    public:

        dicmdCanvasMouseMove() {
            add_option("-point",new PointCommandOptionValue(QPoint(0,0)));
            //FIXME add not initizialize flag
        }
        
        dicmdCanvasMouseMove(const QPoint& p):NonTransactionalDirectCommandBase("-point",new PointCommandOptionValue(p)) 
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


class dicmdCanvasMouseClick: public NonTransactionalDirectCommandBase
{
    
    QPoint m_p;
    public:

        dicmdCanvasMouseClick() {
            add_option("-point",new PointCommandOptionValue(QPoint(0,0)));
        }
        
        dicmdCanvasMouseClick(const QPoint& p):NonTransactionalDirectCommandBase("-point",new PointCommandOptionValue(p)) 
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



//FIXME code duplication
class dicmdCanvasMouseDblClick: public NonTransactionalDirectCommandBase
{
    
    QPoint m_p;
    public:

        dicmdCanvasMouseDblClick() {
            add_option("-point",new PointCommandOptionValue(QPoint(0,0)));
        }
        
        dicmdCanvasMouseDblClick(const QPoint& p):NonTransactionalDirectCommandBase("-point",new PointCommandOptionValue(p)) 
        { m_p = p; }
        
        virtual std::string get_name() {
            return "dicmdCanvasMouseDblClick";
        }
        
        virtual void execute() {
            //m_p = (dynamic_cast<PointCommandOptionValue*>(get_option_val("-point")))->get();
            m_p = GET_CMD_ARG(PointCommandOptionValue,"-point");
            dicmdCanvasMouseMove(m_p).execute();
            QMouseEvent event(QEvent::MouseButtonDblClick, m_p, Qt::LeftButton, 0, 0);
            QApplication::sendEvent(command_manager::get_instance()->get_main_widget()->findChild<QWidget*>("CANVAS"), &event);
        } 
};



#endif
