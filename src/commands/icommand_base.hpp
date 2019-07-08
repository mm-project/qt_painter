#ifndef icommand_base_hpp
#define icommand_base_hpp

#include "../io/messenger.hpp"

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
        
        void log_impl(const std::string& str) {
			Messenger::log_command(str);
		}
		
		virtual void log() {
			CommandBase::log_impl(get_name());
        }
        
        //virtual void activate() {}
        virtual CommandBase* set_arg(const std::string&, const std::string& ) {return 0;}

        //FIXME should not be here !!!
        virtual void handle_mouse_click(int,int) {}
        virtual void handle_mouse_dblclick(int,int) {}
        virtual void handle_mouse_move(int,int) {}
        virtual void handle_key_press() {}
        virtual void handle_update() {}
                
};



#endif
