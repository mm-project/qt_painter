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
#include <QAbstractButton>

#define CM command_manager::get_instance()->get_main_widget()
//fixme SHOULD be templated classes
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
            QComboBox* cmb = CM->findChild<QComboBox*>(m_on.c_str());
            
            std::cout << cmb->currentText().toStdString() << " " << m_on << " " << cmb->findData("White") << std::endl;
            cmb->setCurrentIndex(cmb->findData("White"));
            //btn->click();*/
        }
};


class dicmdguiClickModalButton: public NonTransactionalDirectCommandBase 
{

    std::string m_op1;

    public:
        dicmdguiClickModalButton() {
            add_option("-btn",new StringCommandOptionValue(""));
        }
        
        dicmdguiClickModalButton(const std::string& on):NonTransactionalDirectCommandBase("-btn",new StringCommandOptionValue(on)) 
        { 
            //FIXME add_option()
           // m_on = on; 
        }
        
        virtual std::string get_name() {
            return "dicmdguiClickModalButton";
        }
        
        virtual void execute() {
            QWidget* w = command_manager::get_instance()->get_main_widget();
            m_op1 = GET_CMD_ARG(StringCommandOptionValue,"-btn");
            //m_op2 = GET_CMD_ARG(StringCommandOptionValue,"-in_window");
            QMessageBox* box = w->findChild<QMessageBox*>();
            //box->buttons().back()->click();
            for ( auto it: box->buttons() ) 
                if (  it->text().toStdString() == m_op1  ) {
                    it->click();
                    return;
                }
                    
            //std::cout <<  it->objectName().toStdString() << "  " << it->text().toStdString() << std::endl;
            //QAbstractButton* btn = box->findChild<QAbstractButton*>(m_op1.c_str());
            //btn->click();
        }
};

class dicmdguiClickButton: public NonTransactionalDirectCommandBase 
{

    std::string m_on;
    public:
        dicmdguiClickButton() {
            add_option("-object",new StringCommandOptionValue(""));
        }
        
        dicmdguiClickButton(const std::string& on):NonTransactionalDirectCommandBase("-object",new StringCommandOptionValue(on)) 
        { 
            //FIXME add_option()
            m_on = on; 
        }
        
        virtual std::string get_name() {
            return "dicmdguiClickButton";
        }
        
        virtual void execute() {
            //FIXME add checks
            //FIXME some trick to be more easy?
            m_on = GET_CMD_ARG(StringCommandOptionValue,"-object");
            //m_on = (dynamic_cast<StringCommandOptionValue*>(get_option_val("-object")))->to_string();
            QAbstractButton* btn = CM->findChild<QAbstractButton*>(m_on.c_str());
            btn->click();
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
            QRadioButton* btn = CM->findChild<QRadioButton*>(m_on.c_str());
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
            QApplication::sendEvent(CM->findChild<QWidget*>("CANVAS"), &event);
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
            QApplication::sendEvent(CM->findChild<QWidget*>("CANVAS"), &event);
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
            QApplication::sendEvent(CM->findChild<QWidget*>("CANVAS"), &event);
        } 
};



#endif
