#ifndef basic_commands_hpp
#define basic_commands_hpp

#include "command_manager.hpp"
#include "direct_command_base.hpp"

class incmdIdle : public NonTransactionalDirectCommandBase
{
    public:
        virtual std::string get_name() { return "incmdIdle"; }
        virtual void execute() {} 
};


class dicmdAbortActiveCommand: public NonTransactionalDirectCommandBase
{
    public:        
        virtual std::string get_name() {
            return "dicmdAbortActiveCommand";
        }

        virtual void execute() {
            //command_manager::getInstance().disactivate_active_command();//get_active_command()->abort();
            command_manager::getInstance().get_active_command()->abort();
        }

};


#endif
