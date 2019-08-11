#ifndef interactive_load_save_hpp
#define interactive_load_save_hpp

#include "interactive_command_base.hpp"
#include "load_save_commands.hpp"
#include "command_manager.hpp"

#include "../gui/modal_dialog.hpp"

class incmdDesignLoad: public InteractiveCommandBase
{
    
    IObjectPoolPtr m_ws;
    std::string m_fn;
    
    public:
	
        incmdDesignLoad(IObjectPoolPtr s ):m_ws(s) { 
        }

        virtual std::string get_name() {
                return "incmdDesignLoad";
        }
        
        virtual void execute() {
                set_next_handler(HANDLE_FUNCTION(incmdDesignLoad,on_commit));
        }

        virtual void abort() {
                command_manager::get_instance()->return_to_idle();
        }
		
		
    private:
        void on_commit(const EvType& ev) {
            if (is_agreed_with_user())    
                dicmdDesignLoad(m_ws,m_fn).execute();
            
            abort();
        }

		
        bool is_agreed_with_user() {
                return mmModalDialog::ask_yn_question("DesignLoad","Do you want to continue?");
        }
        
			
};

#endif
