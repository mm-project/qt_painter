#include "log_reader.hpp"


LogReader::LogReader() {
    m_interp = CommandInterp::get_instance();
    m_timer = new QTimer;
}

QStringList LogReader::read_file(const std::string& fname) {
    QStringList stringList;

    QFile file(fname.c_str());
    if(!file.exists() || !file.open(QIODevice::ReadOnly)) {
        //fixme call mmModalDialog , that will also put error message to console and log
        QMessageBox::warning(0, "log replay error", "Can't open file to replay");
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

void LogReader::replay_logfile(const std::string& fname) {
    connect(m_timer, SIGNAL(timeout()), this, SLOT(execute_next_command()));
    
    for (  auto line : read_file(fname)  ) {
        //m_interp->interpret_from_string(line.toStdString());
        m_command_queue.push(line);
        //execute_next_command();
    }
    
    m_timer->start(5);
}

void LogReader::replay_cmd(const std::string& cmd_str ) {
    m_command_queue.push(QString::fromStdString(cmd_str));
    execute_next_command();
}

void LogReader::execute_next_command() {
    //return;
    if (m_command_queue.empty())
        return;
    
    //std::cout << "dolya varavsyaka" << std::endl;
    CommandBase* cmd = m_interp->get_cmd_obj(m_command_queue.front());
    m_command_queue.pop();
    
    if (m_command_queue.empty())
        disconnect(m_timer, 0, 0, 0);
    
    m_interp->execute_cmd(cmd);
    
    //t->deleteLater();
    // QApplication::processEvents();
}
    

