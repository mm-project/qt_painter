#ifndef shapecreationdirective_commands_hpp
#define shapecreationdirective_commands_hpp

#include "direct_command_base.hpp"

#include "../core/shape_creator.hpp"
#include "../core/runtime_environment.hpp"

#include "../io/messenger.hpp"


template <ObjectType T>
class dicmdCreateObj : public DirectCommandBase 
{

        IShape* m_shape;    
        IObjectPoolPtr ws;
        
public:
	dicmdCreateObj<T>(IObjectPoolPtr s): ws(s) {
                add_option("-points",new PointListCommandOptionValue());
        }

       	dicmdCreateObj<T>(const std::vector<PointCommandOptionValue>& pl, IObjectPoolPtr s): ws(s) {
                add_option("-points",new PointListCommandOptionValue(pl));
        }
 
	void silent_execute() {
                execute();
                Messenger::expose(out,DirectCommandBase::get_cmdname_and_stringified_opts(),false);
        }
        
	virtual void execute() {
                //std::vector<QPoint> v(GET_CMD_ARG(PointListCommandOptionValue,"-points"));
                m_shape = ShapeCreator::getInstance()->create(T);
                for( auto it: GET_CMD_ARG(PointListCommandOptionValue,"-points") )
                    m_shape->addPoint(it.get());
                ws->addObject(m_shape);
	}
	
	virtual std::string get_name() {
		return "dicmdCreateObj"+ObjType2String(T);
        }
            
};

#endif
