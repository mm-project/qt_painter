#include "gui/main_window.hpp"
#include "io/log_reader.hpp"
#include "core/application.hpp"

#include <QApplication>

//#include <QtWidgets>
    
int main(int argc, char** argv)
{
	//
	QApplication app(argc, argv);
	main_window window;
	window.show();
        //FIXME ehnance handling cmd args
        if ( argc == 3 ) { // && argv[1] == "-replay" ) {
            Application::get_instance()->set_log_mode(true);
            LogReader r;
            r.replay_log(argv[2]);
        }
        
	return app.exec();
}
