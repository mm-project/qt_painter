#ifndef log_reader_hpp
#define log_reader_hpp

#include "command_interpreter.hpp"

#include "../commands/icommand_base.hpp"
#include "../commands/command_manager.hpp"

#include <QStringList>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QTextStream>
#include <QTimer>
#include <QApplication>

#include <iostream>
#include <queue>


class LogReader : public QObject
{
    Q_OBJECT 
    
    QTimer* m_timer ;
    std::queue<QString> m_command_queue;
    CommandInterp* m_interp;
    
    private:
        QStringList read_file(const std::string& fname);
            
    public:
        LogReader();
        void replay_logfile(const std::string& fname);
        void replay_cmd(const std::string& str);

    private slots:
        void execute_next_command();
        
};

#endif
