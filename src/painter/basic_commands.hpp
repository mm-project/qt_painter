#ifndef basic_commands_hpp
#define basic_commands_hpp

#include "command_manager.hpp"
#include "direct_command_base.hpp"

class incmdIdle : public DirectCommandBase
{
    public:
        virtual std::string get_name() { return "incmdIdle"; }
        virtual void execute() {} 
};


class dicmdAbortActiveCommand: public DirectCommandBase
{
    public:        
        virtual std::string get_name() {
            return "dicmdAbortActiveCommand";
        }

        virtual void execute() {
            command_manager::get_instance()->get_active_command()->abort();
        }

};


#endif
