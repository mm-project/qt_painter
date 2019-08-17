#ifndef interactive_load_save_hpp
#define interactive_load_save_hpp

#include "interactive_command_base.hpp"
#include "load_save_commands.hpp"
#include "command_manager.hpp"

#include "../gui/modal_dialog.hpp"

#include <string>

enum desAction { LOAD, SAVE, NEW, CLOSE };

namespace {
    std::string sl_action2string(desAction a) {
        if ( a == LOAD ) return "Load";
        return "Save";
    }
}

template<desAction T>
class InteractiveDesAction: public InteractiveCommandBase
{
    
    IObjectPoolPtr m_ws;
    std::string m_fn;
    
    public:
	
        InteractiveDesAction<T>(IObjectPoolPtr s ):m_ws(s) { 
        }

        virtual std::string get_name() {
                return "incmdDesign"+sl_action2string(T);
        }
        
        virtual void execute() {
                set_next_handler(HANDLE_FUNCTION(InteractiveDesAction<T>,on_commit));
        }

        virtual void abort() {
                command_manager::get_instance()->return_to_idle();
        }
		
		
    private:
        void on_commit(const EvType& ev) {
            if (is_agreed_with_user())    
                if ( T == LOAD  ) 
                    dicmdDesignLoad(m_ws,m_fn).execute();
                else
                    dicmdDesignSave(m_ws,m_fn).execute();
            abort();
        }

		
        bool is_agreed_with_user() {
                return mmModalDialog::ask_yn_question("Design"+sl_action2string(T),"Do you want to continue?");
        }

			
};

#endif
