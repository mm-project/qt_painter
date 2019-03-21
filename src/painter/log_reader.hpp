#ifndef log_reader_hpp
#define log_reader_hpp

#include "icommand_base.hpp"
#include "command_manager.hpp"

#include <QStringList>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include <iostream>

class LogReader 
{
    
    public:
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
            for (  auto line : read_file(fname)  ) {
                //std::cout << "-" << line.toStdString() << "--" << std::endl;
                QStringList tokens = line.split(" ");
                //std::cout << " --" << tokens[0].toStdString()  << "--" << std::endl;
                
                ICommand* cmd = command_manager::get_instance()->find_command(tokens[0].toStdString());
                
                //if ( cmd->get_type() == Interactive )
                    for (int i=1; i<tokens.size()-1; ++i )
                        //std::cout << "  ---<" << tokens[i].toStdString() << "> <=> <"<< tokens[i+1].toStdString() << ">" << std::endl;
                        cmd->set_arg(tokens[i].toStdString(),tokens[i+1].toStdString());
                
                command_manager::get_instance()->activate_command(dynamic_cast<CommandBase*>(cmd));
            }
            
        }
    
};

#endif
