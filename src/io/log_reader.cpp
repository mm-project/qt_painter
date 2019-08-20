#include "log_reader.hpp"

#include "../core/application.hpp"
#include "../gui/modal_dialog.hpp"

LogReader::LogReader() {
    m_interp = CommandInterp::get_instance();
    m_timer = new QTimer;
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
    connect(m_timer, SIGNAL(timeout()), this, SLOT(execute_next_command()));
    QStringList lines = read_file(fname);
    
    if ( lines.size() == 0 )
        return false;

    std::cout << "noway" << std::endl;
    Application::get_instance()->set_replay_mode(true);
    
    for (  auto line : lines  ) {
        //m_interp->interpret_from_string(line.toStdString());
        m_command_queue.push(line);
        //execute_next_command();
    }
 
    m_timer->start(5);
    return true;
}

void LogReader::replay_cmd(const std::string& cmd_str ) {
    Application::get_instance()->set_replay_mode(true);
    m_command_queue.push(QString::fromStdString(cmd_str));
    execute_next_command();
    Application::get_instance()->set_replay_mode(false);

}

void LogReader::execute_next_command() {
    //return;
    if (m_command_queue.empty()) {
        Application::get_instance()->set_replay_mode(false);
        return;
    }
    
    //std::cout << "dolya varavsyaka" << std::endl;
    CommandBase* cmd = m_interp->get_cmd_obj(m_command_queue.front().toStdString());
    m_command_queue.pop();
    
    if (m_command_queue.empty())
        disconnect(m_timer, 0, 0, 0);
    
    m_interp->execute_cmd(cmd);
    
    //t->deleteLater();
    //QApplication::processEvents();
}
    

