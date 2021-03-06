#ifndef save_laod_command_hpp
#define save_laod_command_hpp

#include "direct_command_base.hpp"
#include "shape_creation_directive_commands.hpp"

#include "../core/runtime_environment.hpp"

#include "../io/log_reader.hpp"

#include <QPoint>

#include <iostream>

class dicmdDesignLoad : public DirectCommandBase 
{
        IObjectPoolPtr ws;

public:
	dicmdDesignLoad(IObjectPoolPtr s): ws(s) {
                add_option("-filename",new StringCommandOptionValue());
        }

        dicmdDesignLoad(IObjectPoolPtr s, const std::string& fname ): ws(s) {
                add_option("-filename",new StringCommandOptionValue(fname));
        }
        
        virtual void execute() {
                ws->clear();
				RegionQuery::getInstance().clear();
                std::string fname(GET_CMD_ARG(StringCommandOptionValue,"-filename"));
                if ( ! LogReader().replay_logfile_imi(fname) )
                    throw 1;
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
                
        //fixme , refactor
        virtual void execute() {
                std::string fname(GET_CMD_ARG(StringCommandOptionValue,"-filename"));
                CommandBase* cmd;
                for ( auto it: ws->getObjects() ) {
                    	switch (it->getType())
                        {
                            case LINE:
                                cmd = new dicmdCreateObj<LINE>(transform(it->getPoints()),it->getProperties(),ws);
                                //cmd->set_arg("-points",PointListCommandOptionValue(transform(it->getPoints())).to_string());
                                dynamic_cast<dicmdCreateObj<LINE>*>(cmd)->dump(fname);
                                break;
                            case RECTANGLE:
                                cmd = new dicmdCreateObj<RECTANGLE>(transform(it->getPoints()),it->getProperties(),ws);
                                //cmd->set_arg("-points",PointListCommandOptionValue(transform(it->getPoints())).to_string());
                                dynamic_cast<dicmdCreateObj<RECTANGLE>*>(cmd)->dump(fname);
                                break;
                            case ELLIPSE:
                                cmd = new dicmdCreateObj<ELLIPSE>(transform(it->getPoints()),it->getProperties(),ws);
                                //cmd->set_arg("-points",PointListCommandOptionValue(transform(it->getPoints())).to_string());
                                dynamic_cast<dicmdCreateObj<ELLIPSE>*>(cmd)->dump(fname);
                                break;
                            case POLYGON:
                                cmd = new dicmdCreateObj<POLYGON>(transform(it->getPoints()),it->getProperties(),ws);
                                //cmd->set_arg("-points",PointListCommandOptionValue(transform(it->getPoints())).to_string());
                                dynamic_cast<dicmdCreateObj<POLYGON>*>(cmd)->dump(fname);
                                break;
                        }

                    //dicmdCreateObj<T>(m_internal_vec,ws).silent_execute();
                    //delete cmd;
                    //cmd = 0;
                    //std::cout << it.getType() << getPoints << std::endl;
                }                    
        }
	
        virtual std::string get_name() {
            return "dicmdDesignSave";
        }
            
    private:
        std::vector<PointCommandOptionValue> transform(const std::vector<QPoint>& v) 
        {
                std::vector<PointCommandOptionValue> res;//(v.size());
                for ( auto it : v ) {
                    res.push_back(PointCommandOptionValue(it));
                }
                return res;
        }

    
};


#endif
