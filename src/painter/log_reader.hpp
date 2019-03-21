#ifndef log_reader_hpp
#define log_reader_hpp

#include "icommand_base.hpp"
#include "command_manager.hpp"

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
    std::queue<ICommand*> m_command_queue;
    
    public:
        LogReader() {
        }
        
        QStringList read_file(const std::string& fname) {
            QStringList stringList;

            QFile file(fname.c_str());
            if(!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(0, "error", file.errorString());
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
        
        void replay_log(const std::string& fname) {
            timer = new QTimer(this);
            //connect(timer, SIGNAL(timeout()), this, SLOT(execute_command()));
            //timer->start(10);

            for (  auto line : read_file(fname)  ) {
                //std::cout << "-" << line.toStdString() << std::endl;
                QStringList tokens = line.split(" ");
                //std::cout << " --" << tokens[0].toStdString()  << "--" << std::endl;
                
                ICommand* cmd = command_manager::get_instance()->find_command(tokens[0].toStdString());
                
                //if ( cmd->get_type() == Interactive )
                    for (int i=1; i<tokens.size()-1; ++i ) {
                        //std::cout << "  ---<" << tokens[i].toStdString() << "> <=> <"<< tokens[i+1].toStdString() << ">" << std::endl;
                        cmd->set_arg(tokens[i].toStdString(),tokens[i+1].toStdString());
                    }
                
                m_command_queue.push(cmd);
                execute_command();
            }
            
        }
        
        public slots:
            void execute_command() {
                
                    //return;
                if (m_command_queue.empty())
                    return;
                
                //std::cout << "dolya varavsyaka" << std::endl;
                ICommand* cmd = m_command_queue.front();
                m_command_queue.pop();
                
                if (m_command_queue.empty())
                    disconnect(timer, 0, 0, 0);
                
                
                if ( cmd->get_type() == Interactive )    
                    command_manager::get_instance()->activate_command(dynamic_cast<CommandBase*>(cmd));
                else
                    cmd->execute_and_log();
                
                //t->deleteLater();
                QApplication::processEvents();
            }
    
};

#endif
