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
    //fixmeeeeeeee
    std::string sl_action2string(desAction a) {
        if ( a == LOAD ) return "Load";
        if ( a == SAVE ) return "Save";
        if ( a == NEW )  return "New";
        if ( a == CLOSE ) return "Close";
		return "";
    }
}

template<desAction T>
class InteractiveDesAction: public InteractiveCommandBase
{
    
    IObjectPoolPtr m_ws;
    std::string m_fn;
    std::string m_helpstr;
    bool m_is_saved = false;
    
    public:
	
        InteractiveDesAction<T>(IObjectPoolPtr s ):m_ws(s) { 
                m_helpstr = sl_action2string(T);
        }

        virtual std::string get_name() {
                return "incmdDesign"+m_helpstr;
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
            if ( T == LOAD  ) {
                if ( is_agreed_with_user() )  {
                    m_fn = QFileDialog::getOpenFileName(0,"Load Design","","*.*",0,QFileDialog::DontUseNativeDialog).toStdString();
                    dicmdDesignLoad(m_ws,m_fn).silent_execute();
                }
            } else if ( T == SAVE ) {
                 if ( is_agreed_with_user() ) {  
                    m_fn = QFileDialog::getSaveFileName(0,"Save Design","","*.*",0,QFileDialog::DontUseNativeDialog).toStdString();
                    dicmdDesignSave(m_ws,m_fn).silent_execute();
                    m_is_saved = true;
                 }
            } else if ( T == NEW ) {
                if ( is_agreed_with_user() ) {    
                    m_ws->clear();
                    command_manager::get_instance()->get_main_widget()->update();
                }
            }
            abort();
        }

        bool is_saved() {
                return m_is_saved;
        }
        
        bool is_agreed_with_user() {
            //if ( ! is_saved() )    
                //return mmModalDialog::ask_yn_question("Design"+m_helpstr,"Your design has not been saved yet. Do you want to continue with "+m_helpstr+"?");
            return true;
        }

			
};

#endif
