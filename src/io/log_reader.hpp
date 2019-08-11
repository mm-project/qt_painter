#ifndef log_reader_hpp
#define log_reader_hpp

#include "../commands/icommand_base.hpp"
#include "../commands/command_manager.hpp"

#include <QStringList>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTimer>
#include <QApplication>

#include <iostream>
#include <queue>


class LogReader : public QObject
{
    Q_OBJECT 
    
    QTimer* timer ;
    std::queue<CommandBase*> m_command_queue;
    
    private:
        QStringList read_file(const std::string& fname);
            
    public:
        LogReader();
        void replay_log(const std::string& fname);
        void replay_command(const std::string& cmd);
        
    public slots:
        void execute_next_command();
    
};

#endif
