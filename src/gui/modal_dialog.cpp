#include "modal_dialog.hpp"

#include "../io/messenger.hpp"
#include "../core/application.hpp"
#include "../commands/gui_commands.hpp"
#include "../commands/command_manager.hpp"

#include <QMessageBox>

#include <iostream>

bool mmModalDialog::ask_yn_question(const std::string& tl, const std::string& bd) 
{
    QWidget* m_parent_w = command_manager::get_instance()->get_main_widget();
    if ( Application::is_replay_mode() ) {
        QMessageBox* m_box = new QMessageBox(m_parent_w);   
        m_box->setIcon(QMessageBox::Question);
        m_box->setText(tl.c_str());
        m_box->setInformativeText(bd.c_str());
        m_box->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        m_box->setDefaultButton(QMessageBox::Cancel);
        m_box->show();
        //fixme how to delete?
    } else {
        Messenger::expose_msg(info,bd);
        QMessageBox::StandardButton reply = QMessageBox::question(m_parent_w, tl.c_str() , bd.c_str());
        if (reply == QMessageBox::Yes) {
            dicmdguiClickModalButton("&Yes").log();
            return true;
        } else {
            dicmdguiClickModalButton("&Cancel").log();        
            return false;
        }
    
    }
	return false; // warning
}


bool mmModalDialog::critical(const std::string& tl, const std::string& bd) 
{
    QWidget* m_parent_w = command_manager::get_instance()->get_main_widget();
    if ( Application::is_replay_mode() ) {
        QMessageBox* m_box = new QMessageBox(m_parent_w);   
        m_box->setText(tl.c_str());
        m_box->setInformativeText(bd.c_str());
        m_box->setStandardButtons(QMessageBox::Ok);
        m_box->setDefaultButton(QMessageBox::Ok);
        m_box->show();
        //fixme how to delete?
    } else {
        Messenger::expose_msg(err,bd);
        QMessageBox::StandardButton reply = QMessageBox::critical(m_parent_w, tl.c_str() , bd.c_str());
        dicmdguiClickModalButton("&Ok").log();
    }
	return true;
}

//fixme need to have single actual invocation
/*
bool mmModalDialog::internal(const std::string& tl, const std::string& bd) 
{
}
*/
