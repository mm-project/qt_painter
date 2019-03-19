#ifndef icommand_base_hpp
#define icommand_base_hpp

#include <string>

class ICommand
{
    public:
        virtual void execute_and_log() {
            log(get_name());
            execute();
        }
        
        virtual void log(const std::string&) = 0;
        virtual void execute() = 0;
        virtual std::string get_name() = 0;
        
        
        //FIXME bug, pure virtual dtor makes compiler sad:/
        virtual ~ICommand() {}
};

#endif
