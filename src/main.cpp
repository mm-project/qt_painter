#include "core/application.hpp"
#include "core/stackdump.hpp"
#include "gui/main_window.hpp"
#include "io/log_reader.hpp"
#include "io/command_interpreter.hpp"

#include <QApplication>
#include <QTimer>
#include <QThread>

#ifdef OS_LINUX
#include <signal.h>
#endif

// fixme put to application class
void handle_replaying(const std::string &fname)
{
    LogReader *r = new LogReader;
    r->replay_logfile(fname);
    // fixme how deletes?
    // delete r;
}

void handle_python(const std::string &fname)
{
    CommandInterp& ci = CommandInterp::getInstance();
    ci.interpret_from_file(fname);
}

void hande_commandline_options(int argc, char **argv)
{
    if (argc == 3)
    {
        char* instr = argv[1];

        if (strcmp(instr,"-replay") == 0)
            handle_replaying(argv[2]);

        if (strcmp(instr,"-exec_python") == 0)
            handle_python(argv[2]);
    }
}

int main2(int argc, char **argv)
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
    // FIXME ehnance handling cmd args
    hande_commandline_options(argc, argv);
    return app.exec();
}

class PythonRunner : public QThread {
    //Q_OBJECT
public:
    explicit PythonRunner(std::string script, QObject* parent = nullptr)
        : QThread(parent), script_(std::move(script)) {}

protected:
    void run() override {
        handle_python(script_);
    }

private:
    std::string script_;
};

int main(int argc, char **argv)
{
//#ifdef OS_LINUX
    signal(SIGSEGV, handler);
    signal(SIGILL, handler);
    signal(SIGFPE, handler);
    signal(SIGTERM, handler);
    signal(SIGABRT, handler);
//#endif
    QApplication app(argc, argv);
    main_window window;
    window.show();

if (argc == 3 && strcmp(argv[1], "-exec_python") == 0) {
    std::string script = argv[2];
    QTimer::singleShot(0, [script]() {
        PythonRunner* runner = new PythonRunner(script);
        runner->start();
    });
} else {
    hande_commandline_options(argc, argv);
}
    return app.exec();
}