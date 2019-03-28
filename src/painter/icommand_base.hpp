#ifndef icommand_base_hpp
#define icommand_base_hpp

#include <string>
#include <iostream>

enum CommandType { Interactive = 0, Directive };

class ICommand
{
    public:

        //virtual void pre_execute() = 0;
        virtual void execute() = 0;
        //virtual void post_execute() = 0;
        virtual void abort() = 0;
        virtual void log() = 0;
        virtual std::string get_name() = 0;
        virtual CommandType get_type() = 0;
        //FIXME
    
        //FIXME
        virtual void execute_and_log() = 0;
        
        //FIXME bug, pure virtual dtor makes compiler sad:/
        virtual ~ICommand() {}
};


class CommandBase : public ICommand
{
    public:
        virtual void execute_and_log() {
            log();
            try {
                //pre_execute(); 
                execute();
                //post_execute() 
            } catch(...) {
                std::cout << " Error: Fixme Exception or Error?? " << std::endl;
            }
        }
        
        virtual void log() {
            //if ( ... )
            std::cout << get_name() << std::endl;
        }
        
		virtual void activate() {}
        virtual void set_arg(const std::string&, const std::string& ) {}

        //FIXME should not be here !!!
        virtual void handle_mouse_click(int,int) {}
        virtual void handle_mouse_dblclick(int,int) {}
        virtual void handle_mouse_move(int,int) {}
        virtual void handle_key_press() {}
        virtual void handle_update() {}
                
};



#endif
