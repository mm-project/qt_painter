#ifndef application_hpp
#define application_hpp

#include "service.h"
#include "../io/messenger.hpp"

#include <QString>

enum appMode {APPNORM, APPREPLAY, APPTESTING, APPLOAD};

class Application: public Service<Application> //, public QApplication
{
    appMode m_mode;
    public:
        //using QApplication::QApplication;
        Application() { m_mode = APPNORM; }
        
        void set_replay_mode(bool m) { 
            Messenger::expose_msg(info,"Set to mode"+QString::number(int(m)).toStdString());
            m_mode = m?APPREPLAY:APPNORM; 
        }
        
        void set_mode(const appMode& m) { 
            Messenger::expose_msg(info,"Set to mode"+QString::number(int(m)).toStdString());
            m_mode = m; 
        } 
        
        appMode get_mode() { return m_mode; }
        
    public:
        /*
        bool notify(QObject *receiver, QEvent *event) {
            if( QWidget *button = qobject_cast<QWidget *>(receiver))
                if(event->type() == QEvent::MouseButtonPress)
                    qDebug()<< button->objectName();
            return QApplication::notify(receiver, event);
        }
        /**/
        
        static bool is_replay_mode() { return ( Application::get_instance()->get_mode() == APPREPLAY); }
        static bool is_testing_mode() { return (Application::get_instance()->get_mode() == APPTESTING); }
        static bool is_load_mode() { return (Application::get_instance()->get_mode() == APPLOAD); }
        
};


#endif
