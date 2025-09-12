#include "messenger.hpp"

#include "../core/application.hpp"
#include "../core/callback.hpp"
#include "../core/postman.hpp"

#include <QDateTime>
#include <QString>

#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>

Messenger::Messenger()
{
    init();
}

Messenger::~Messenger()
{
    fini();
}

void Messenger::fini()
{
    m_cmdfile->close();
    m_logfile->close();
}

void Messenger::init()
{

    QString pathenv = QString::fromLocal8Bit(qgetenv("PAINTER_LOGS_DIR").constData());
    QString idpostfix = QString::fromLocal8Bit(qgetenv("PAINTER_LOGFILE_PREFIX").constData());

    QString m_path = pathenv.isEmpty() ? "./logs/" : pathenv;

    QString postfix = "painter" + QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");
    postfix = idpostfix.isEmpty() ? postfix : idpostfix;
    // std::cout << id.toStdString() << std::endl;

    m_dir = new QDir; //;("logs");

    if (!m_dir->exists(m_path))
        m_dir->mkpath(m_path); // You can check the success if needed

    m_cmdfile = new QFile(m_path + postfix + ".lvi");
    m_logfile = new QFile(m_path + postfix + ".log");

    /*
    m_cmdfile->open( QIODevice::WriteOnly | QIODevice::Append );
    m_logfile->open( QIODevice::WriteOnly | QIODevice::Append );

    log_stream = new QTextStream(m_logfile);
    cmd_stream = new QTextStream(m_cmdfile);
    */
}

std::string Messenger::decorate_for_logging(const LogMsgSeverity &r)
{

    switch (r)
    {
    case ok:
        return ("");
    case log_:
        return ("#r ");
    case usr:
        return ("#u --> User: ");
    case err:
        return ("#e --> Error: ");
        break;
    case info:
        return ("#i --> Info: ");
    case warn:
        return ("#w --> Warning: ");
    case out:
        return ("#o --> Out: ");
    case cont:
        return ("#c ");
    case test:
        return ("#t --> Test: ");
    case modal:
        return ("#m ");
    default:
        return ("#? ");
        break;
    }
}

// FIXME
void Messenger::expose_internal(const LogMsgSeverity &severity, const std::string &m, bool iscmd)
{
    LeCallbackData fixme;
    if ((severity == warn || severity == err) &&
        (!QString::fromLocal8Bit(qgetenv("ELEN_PAINTER_STARTDBG").constData()).isEmpty()))
        NOTIFY(STOP_REPLY, fixme);

    if (Application::is_load_mode())
        return;

    QString lines(m.c_str());
    for (auto line : lines.split("\n"))
    {
        std::string msg = line.toStdString();
        write_entry_to_console_gui(severity, msg);

        std::stringstream z;
        z << decorate_for_logging(severity) << msg << "\n";
        write_entry_to_logfile(z.str());

        // if this is <real> command, write also to lvi file.
        if (iscmd)
            write_entry_to_cmdfile(msg);
    }
}

void Messenger::write_entry_to_console_gui(const LogMsgSeverity &s, const std::string &msg)
{
    std::cout << msg << std::endl;

    std::string errcode = "";
    if (s != ok)
        errcode = "PROJ-001";

    MessengerCallbackData data(s, msg, errcode);
    NOTIFY(MESSENGER, data);
}

// fixme duplicates
void Messenger::write_entry_to_logfile(const std::string &msg)
{
    m_logfile->open(QIODevice::WriteOnly | QIODevice::Append);
    log_stream = new QTextStream(m_logfile);
    (*log_stream) << msg.c_str();
    m_logfile->flush();
    m_logfile->close();
}

void Messenger::write_entry_to_cmdfile(const std::string &msg)
{
    m_cmdfile->open(QIODevice::WriteOnly | QIODevice::Append);
    cmd_stream = new QTextStream(m_cmdfile);
    (*cmd_stream) << (msg + "\n").c_str();
    m_cmdfile->flush();
    m_cmdfile->close();
}

// static
void Messenger::expose_msg(const LogMsgSeverity &s, const std::string &msg, bool iscmd)
{

    Messenger::getInstance().expose_internal(s, msg, iscmd);
}

// static used by CommandBase internally , fixme add friend
void Messenger::log_command(const std::string &msg, bool iscmd)
{
    //if(! Application::is_replay_mode())
		Messenger::expose_msg(ok, msg, iscmd);
    //else
        //Messenger::expose_msg(log_,msg,iscmd);
}

// Messenger::expose(err,"Error: ... ")
