#ifndef save_laod_command_hpp
#define save_laod_command_hpp

#include "direct_command_base.hpp"
#include "shape_creation_directive_commands.hpp"

#include "../core/runtime_environment.hpp"

#include "../io/log_reader.hpp"

#include <QPoint>
#include <QMessageBox>

class dicmdDesignLoad : public DirectCommandBase 
{
        IObjectPoolPtr ws;

        bool is_agreed_with_user(const std::string& tl, const std::string& bd) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(0, tl.c_str() , bd.c_str());//QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes) 
                    return true;
                
                return false;
        }
        
public:
	dicmdDesignLoad(IObjectPoolPtr s): ws(s) {
                add_option("-filename",new StringCommandOptionValue());
        }

        dicmdDesignLoad(IObjectPoolPtr s, const std::string& fname ): ws(s) {
                add_option("-filename",new StringCommandOptionValue(fname));
        }
        
	virtual void execute() {
                
                if ( ! is_agreed_with_user("Loading design","Do you want to discard changes?") ) 
                    return ;
                
                ws->clear();
                std::string fname(GET_CMD_ARG(StringCommandOptionValue,"-filename"));
                LogReader().replay_log(fname);
	}
	
	virtual std::string get_name() {
		return "dicmdDesignLoad";
        }
};


class dicmdDesignSave : public DirectCommandBase 
{

        IObjectPoolPtr ws;
        
public:
	dicmdDesignSave(IObjectPoolPtr s): ws(s) {
                add_option("-filename",new StringCommandOptionValue());
        }

        dicmdDesignSave(IObjectPoolPtr s, const std::string& fname ): ws(s) {
                add_option("-filename",new StringCommandOptionValue(fname));
        }
        
	std::vector<PointCommandOptionValue> transform(const std::vector<QPoint>& v) {
                std::vector<PointCommandOptionValue> res(v.size());
                for ( auto it: v ) 
                    res.push_back(PointCommandOptionValue(it));
                return res;
        }
        
	virtual void execute() {
                std::string fname(GET_CMD_ARG(StringCommandOptionValue,"-filename"));
                for ( auto it: ws->getObjects() ) {
                    //dicmdCreateObj<T>(m_internal_vec,ws).silent_execute();
                    CommandBase* cmd = new dicmdCreateObj<RECTANGLE>(ws);
                    cmd->set_arg("-points",PointListCommandOptionValue(transform(it->getPoints())).to_string());
                    
                    dynamic_cast<dicmdCreateObj<RECTANGLE>*>(cmd)->dump(fname);
                    delete cmd;
                    cmd = 0;
                    //std::cout << it.getType() << getPoints << std::endl;
                }                    
	}
	
	virtual std::string get_name() {
		return "dicmdDesignSave";
        }
            
};


#endif
