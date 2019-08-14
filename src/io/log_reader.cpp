#include "log_reader.hpp"


LogReader::LogReader() {
    m_interp = CommandInterp::get_instance();
}

QStringList LogReader::read_file(const std::string& fname) {
    QStringList stringList;

    QFile file(fname.c_str());
    if(!file.exists() || !file.open(QIODevice::ReadOnly)) {
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
    
    return stringList;
}

void LogReader::replay_log(const std::string& fname) {
    timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(execute_command()));
    //timer->start(1);
    for (  auto line : read_file(fname)  ) {
        //m_interp->interpret_from_string(line.toStdString());
        m_command_queue.push(m_interp->get_cmd_obj(line));
        execute_next_command();
    }
    
}

void LogReader::execute_next_command() {
    //return;
    if (m_command_queue.empty())
        return;
    
    //std::cout << "dolya varavsyaka" << std::endl;
    CommandBase* cmd = m_command_queue.front();
    m_command_queue.pop();
    
    if (m_command_queue.empty())
        disconnect(timer, 0, 0, 0);
    
    m_interp->execute_cmd(cmd);
    
    //t->deleteLater();
    // QApplication::processEvents();
}
    

