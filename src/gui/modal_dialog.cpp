#include "modal_dialog.hpp"
#include "../commands/gui_commands.hpp"

bool mmModalDialog::ask_yn_question(const std::string& tl, const std::string& bd) {
        mmModalDialog m;
        QMessageBox::StandardButton reply;
        reply = mmModalDialog::question(0, tl.c_str() , bd.c_str());
        if (reply == QMessageBox::Yes) {
            dicmdguiPressButton("Yes",tl).log();
            return true;
        } else {
            dicmdguiPressButton("No",tl).log();        
            return false;
        }
}

int mmModalDialog::exec() {
        //REGISTER_CALLBACK(INTERACTIVE_COMMAND_PRE_COMMIT, &Messenger::test1);
        QMessageBox::show();
        //NOTIFY(INTERACTIVE_COMMAND_PRE_COMMIT,a)
        
        return 0;
        //return QMessageBox::exec();
}
        
