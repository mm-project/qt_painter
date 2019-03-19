#ifndef icommand_base_hpp
#define icommand_base_hpp

#include <string>
#include <iostream>

enum CommandType { Interactive = 0, Directive };

class ICommand
{
    public:

        virtual void execute_and_log() = 0;
        virtual void log(const std::string&) = 0;
        virtual void execute() = 0;
        virtual void abort() = 0;
        virtual std::string get_name() = 0;
        virtual CommandType get_type() = 0;
        
        //FIXME bug, pure virtual dtor makes compiler sad:/
        virtual ~ICommand() {}
};


class CommandBase : public ICommand
{
    public:
        virtual void execute_and_log() {
            log(get_name());
            //pre_execute();
            execute();
            //post_execute();
        }
        
        virtual void log(const std::string& msg) {
            //if ( ... )
            std::cout << msg << std::endl;
        }
        
        //FIXME should not be here !!!
        virtual void handle_mouse_click(int,int) {}
        virtual void handle_mouse_dblclick(int,int) {}
        virtual void handle_mouse_move(int,int) {}
        virtual void handle_key_press() {}
        virtual void handle_update() {}
                
};



#endif
