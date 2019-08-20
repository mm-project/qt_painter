#ifndef modal_dialog_hpp
#define modal_dialog_hpp

#include "../core/postman.hpp"
#include "../commands/command_manager.hpp"

#include <string>

#include <QMessageBox>

class mmModalDialog : public QMessageBox 
{
    Q_OBJECT
    
    public:
        static bool ask_yn_question(const std::string& tl, const std::string& bd);

    public:
        mmModalDialog():QMessageBox(command_manager::get_instance()->get_main_widget()) {
        }
        
        virtual int exec();
        
    private:
};

#endif
