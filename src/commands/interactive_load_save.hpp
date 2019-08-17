#ifndef interactive_load_save_hpp
#define interactive_load_save_hpp

#include "interactive_command_base.hpp"
#include "load_save_commands.hpp"
#include "command_manager.hpp"

#include "../gui/modal_dialog.hpp"

#include <string>

enum SL_ACTION { LOAD, SAVE };

namespace {
    std::string sl_action2string(SL_ACTION a) {
        if ( a == LOAD ) return "Load";
        return "Save";
    }
}

template<SL_ACTION T>
class InteractiveLoadSave: public InteractiveCommandBase
{
    
    IObjectPoolPtr m_ws;
    std::string m_fn;
    
    public:
	
        InteractiveLoadSave<SL_ACTION>(IObjectPoolPtr s ):m_ws(s) { 
        }

        virtual std::string get_name() {
                return "incmdDesign"+sl_action2string(SL_ACTION);
        }
        
        virtual void execute() {
                set_next_handler(HANDLE_FUNCTION(InteractiveLoadSave<SL_ACTION>,on_commit));
        }

        virtual void abort() {
                command_manager::get_instance()->return_to_idle();
        }
		
		
    private:
        void on_commit(const EvType& ev) {
            if (is_agreed_with_user())    
                if ( SL_ACTION == load ) 
                    dicmdDesignLoad(m_ws,m_fn).execute();
                else
                    dicmdDesignSave(m_ws,m_fn).execute();
            abort();
        }

		
        bool is_agreed_with_user() {
                return mmModalDialog::ask_yn_question("DesignLoad","Do you want to continue?");
        }
        
			
};

#endif
