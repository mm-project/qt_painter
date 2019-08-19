#include "gui/main_window.hpp"
#include "io/log_reader.hpp"
#include "core/application.hpp"

#include <QApplication>

//#include <QtWidgets>
    
//fixme put to application class    
void handle_replaying(const std::string& fname)
{
        Application::get_instance()->set_replay_mode(true);
        LogReader* r = new LogReader;
        r->replay_logfile(fname);
        //delete r;
        Application::get_instance()->set_replay_mode(false);
}

void hande_commandline_options(int argc, char** argv)
{
        if ( argc == 3 ) { // && argv[1] == "-replay" ) {
            handle_replaying(argv[2]);
        }
}

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	main_window window;
	window.show();
        //FIXME ehnance handling cmd args
        hande_commandline_options(argc,argv);
	return app.exec();
}
