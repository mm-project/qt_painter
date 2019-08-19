#ifndef application_hpp
#define application_hpp

#include "service.h"

class Application: public Service<Application> //, public QApplication
{
    bool m_mode;
    public:
        //using QApplication::QApplication;
        Application() { m_mode = false; }
        void set_replay_mode(bool m) { m_mode = m; }
        bool get_mode() { return m_mode; }
        
    public:
        /*
        bool notify(QObject *receiver, QEvent *event) {
            if( QWidget *button = qobject_cast<QWidget *>(receiver))
                if(event->type() == QEvent::MouseButtonPress)
                    qDebug()<< button->objectName();
            return QApplication::notify(receiver, event);
        }
        /**/
        
        static bool is_log_mode() { return Application::get_instance()->get_mode(); }
};


#endif
