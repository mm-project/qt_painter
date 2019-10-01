#ifndef command_interpreter_hpp
#define command_interpreter_hpp

#include "messenger.hpp"

#include "../core/service.h"
#include "../commands/command_manager.hpp"
#include "../commands/icommand_base.hpp"
#include "../commands/direct_command_base.hpp"



#include <QStringList>
#include <QString>

#include <string>
#include <iostream>

class CommandInterp : public Service<CommandInterp>
{
    command_manager* m_cm;
    
    public:
        CommandInterp() {
            m_cm = command_manager::get_instance();
        }
        
        bool interpret_from_string(const std::string& n) {
            execute_cmd(get_cmd_obj(n));
            return true;
        }
        
        
        CommandBase* get_cmd_obj(const std::string& n) {
            QStringList tokens = QString(n.c_str()).split(" ");  
            std::string cmd_name = tokens[0].toStdString();
            //std::cout << "<"<<n.toStdString()<<">" << tokens[0].toStdString() << std::endl;
            if ( cmd_name.front() == '#' )
                return 0;
            
            CommandBase* cmd = command_manager::get_instance()->find_command(cmd_name);
            if ( !cmd ) {
                Messenger::expose_msg(usr,cmd_name);
                Messenger::expose_msg(err,cmd_name+" command not found"); //FIXME enhance with message.err file
                return 0;
            }
                
            //std::cout << cmd << std::endl;
            //std::cout << cmd->get_name() << std::endl;
            //std::cout << cmd->get_type() << std::endl;
            
            //fixme parsing
            for (int i=1; i<tokens.size()-1; i=i+2 ) {
                //std::cout << "  ---<" << tokens[i].toStdString() << "> <=> <"<< tokens[i+1].toStdString() << ">" << std::endl;
                if ( ! cmd->set_arg(tokens[i].toStdString(),tokens[i+1].toStdString()) ) {
                    Messenger::expose_msg(usr,cmd_name+" "+tokens[i].toStdString()+" "+tokens[i+1].toStdString());
                    Messenger::expose_msg(err,tokens[i].toStdString()+" argument is in invalid ");
                    return 0;
                }
            }
            
            return cmd;
                    
        }
        
        void execute_cmd(CommandBase* cmd) {
            if(!cmd)
                return;
            //std::cout << "<"<<n.toStdString()<<">" << tokens[0].toStdString() << std::endl;
            
            if ( cmd->get_type() == Interactive )    
                m_cm->activate_command(dynamic_cast<CommandBase*>(cmd));
            else
                cmd->execute_and_log();
                //std::cout  << "!!!!" << dynamic_cast<DirectCommandBase*>(cmd)->get_cmdname_and_stringified_opts() << std::endl;
                
        }
};

#endif
