#include "gui/main_window.hpp"
#include "io/log_reader.hpp"
#include "core/application.hpp"
#include "core/stackdump.hpp"

#include <QApplication>

#ifdef OS_LINUX
    #include <signal.h>
#endif
    
//fixme put to application class    
void handle_replaying(const std::string& fname)
{
        LogReader* r = new LogReader;
        r->replay_logfile(fname);
        //fixme how deletes?
        //delete r;
}

void hande_commandline_options(int argc, char** argv)
{
        if ( argc == 3 ) { // && argv[1] == "-replay" ) {
            handle_replaying(argv[2]);
        }
}

int main(int argc, char** argv)
{
#ifdef OS_LINUX
        signal(SIGSEGV, handler);   
        signal(SIGILL, handler);   
        signal(SIGFPE, handler); 
        signal(SIGTERM, handler);
        signal(SIGABRT, handler);
#endif
        QApplication app(argc, argv);
	main_window window;
	window.show();
        //FIXME ehnance handling cmd args
        hande_commandline_options(argc,argv);
	return app.exec();
}
