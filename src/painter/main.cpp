#include "main_window.hpp"
#include "log_reader.hpp"

#include <QApplication>

//#include <QtWidgets>
    
/*
class Application: public QApplication
{
    public:
        using QApplication::QApplication;
    public:
        bool notify(QObject *receiver, QEvent *event) {
            if( QWidget *button = qobject_cast<QWidget *>(receiver))
                if(event->type() == QEvent::MouseButtonPress)
                    qDebug()<< button->objectName();
            return QApplication::notify(receiver, event);
        }
};
*/

int main(int argc, char** argv)
{
	//
	QApplication app(argc, argv);
	main_window window;
	window.show();
        //FIXME ehnance handling cmd args
        if ( argc == 3 ) { // && argv[1] == "-replay" ) {
            LogReader r;
            r.replay_log(argv[2]);
        }
        
	return app.exec();
}
