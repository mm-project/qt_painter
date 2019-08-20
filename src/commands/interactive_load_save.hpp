#ifndef interactive_load_save_hpp
#define interactive_load_save_hpp

#include "interactive_command_base.hpp"
#include "load_save_commands.hpp"
#include "command_manager.hpp"

#include "../gui/modal_dialog.hpp"

#include<QFileDialog>

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
    bool m_is_saved = false;
    
    public:
	
        InteractiveDesAction<T>(IObjectPoolPtr s ):m_ws(s) { 
        }

        virtual std::string get_name() {
                return "incmdDesign"+sl_action2string(T);
        }
        
        virtual void execute() {
                //set_next_handler(HANDLE_FUNCTION(InteractiveDesAction<T>,on_commit));
                on_commit(OTHER);
        }

        virtual void abort() {
                command_manager::get_instance()->return_to_idle();
        }
		
		
    private:
        void on_commit(const EvType& ev) {
            m_fn = "test.txt"; 
            if ( T == LOAD  ) {
                if ( is_agreed_with_user() )    
                    dicmdDesignLoad(m_ws,m_fn).silent_execute();
                    //m_fn = QFileDialog::getOpenFileName(command_manager::get_instance()->get_main_widget(),"aaa","aaa","*.*").toStdString();
            } else {
                 if ( is_agreed_with_user() )    
                    //m_fn = QFileDialog::getSaveFileName(command_manager::get_instance()->get_main_widget(),"aaa","aaa","*.*").toStdString();
                     dicmdDesignSave(m_ws,m_fn).silent_execute();
                     m_is_saved = true;
            }
            abort();
        }

        bool is_saved() {
                return m_is_saved;
        }
        
        bool is_agreed_with_user() {
            if ( ! is_saved() )    
                return mmModalDialog::ask_yn_question("Design"+sl_action2string(T),"Your design has not been saved yet. Do you want to continue with "+sl_action2string(T)+"?");
            return true;
        }

			
};

#endif
