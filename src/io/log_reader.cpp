#include "log_reader.hpp"
#include "messenger.hpp"

#include "../core/application.hpp"
#include "../core/postman.hpp"
#include "../gui/modal_dialog.hpp"

#include <iostream>

LogReader::LogReader() {
    m_interp = CommandInterp::get_instance();
    m_timer = new QTimer;
    REGISTER_CALLBACK(STOP_REPLY,&LogReader::reply_stop)
    REGISTER_CALLBACK(RESUME_REPLY,&LogReader::reply_resume)
    REGISTER_CALLBACK(STEP_REPLY,&LogReader::step_reply)
    
}


LogReader::~LogReader() {
    //std::cout << "germania text << std::endl;
}


void LogReader::step_reply(LeCallbackData&) 
{
    //Application::get_instance()->set_replay_mode(true);
    execute_next_command();
    //Application::get_instance()->set_replay_mode(false);    
}

void LogReader::reply_resume(LeCallbackData&) 
{
    std::cout << "recv" << std::endl;
    m_paused = false;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(execute_next_command()));
    Application::get_instance()->set_replay_mode(true);
    m_timer->start(1);    
}


void LogReader::reply_stop(LeCallbackData&) 
{
    m_paused = true;   
    disconnect(m_timer, 0, 0, 0);
    Application::get_instance()->set_replay_mode(false);
    Messenger::expose_msg(info,"Replying Stopped for debug. Press \"W\" to continue to run till next breakpoint, or \"S\" to execute next command.");
}

bool LogReader::is_paused() 
{
    return m_paused;   
}


QStringList LogReader::read_file(const std::string& fname) {
    QStringList stringList;

    QFile file(fname.c_str());
    if(!file.exists() || !file.open(QIODevice::ReadOnly)) {
        //fixme call mmModalDialog , that will also put error message to console and log
        Application::get_instance()->set_replay_mode(false);
        mmModalDialog::critical("Log replay error", "Can't open file "+fname+" to replay");
        return stringList;
    }
    
    QTextStream textStream(&file);
    while (true)
    {
        QString line = textStream.readLine();
        if (line.isNull())
            break;
        else
            stringList.append(line);
    }
    
    file.close();
    return stringList;
}

bool LogReader::replay_logfile(const std::string& fname) {
    //connect(m_timer, SIGNAL(timeout()), this, SLOT(execute_next_command()));
    QStringList lines = read_file(fname);
    
    if ( lines.size() == 0 )
        return false;

    //std::cout << "noway" << std::endl;
    
    
    for (  auto line : lines  ) {
        //m_interp->interpret_from_string(line.toStdString());
        m_command_queue.push(line);
        //execute_next_command();
    }
 
    reply_resume(fixme);
    //m_timer->start(1);
    return true;
}

bool LogReader::replay_logfile_imi(const std::string& fname) {
    QStringList lines = read_file(fname);
    
    if ( lines.size() == 0 )
        return false;

    //std::cout << "noway" << std::endl;
    Application::get_instance()->set_mode(APPLOAD);
    
    for (  auto line : lines  ) {
        replay_cmd(line.toStdString());
    }

    //fixme
    Application::get_instance()->set_replay_mode(false);

    return true;    
}

void LogReader::replay_cmd(const std::string& cmd_str ) {
    //Application::get_instance()->set_replay_mode(true);
    m_command_queue.push(QString::fromStdString(cmd_str));
    execute_next_command();
    //Application::get_instance()->set_replay_mode(false);

}

void LogReader::execute_next_command() {
    //return;
    if (m_command_queue.empty()) {
        reply_stop(fixme);
        return;
    }
    
    std::cout << "dolya varavsyaka" << std::endl;
    CommandBase* cmd = m_interp->get_cmd_obj(m_command_queue.front().toStdString());
    m_command_queue.pop();
    
    //if (m_command_queue.empty()) {
    //    reply_stop(fixme);
    //    return;
    //}
        //disconnect(m_timer, 0, 0, 0);
    
    m_interp->execute_cmd(cmd);
    
    //t->deleteLater();
    //QApplication::processEvents();
}
    

