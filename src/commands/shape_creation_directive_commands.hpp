#ifndef shapecreationdirective_commands_hpp
#define shapecreationdirective_commands_hpp

#include "direct_command_base.hpp"

#include "../core/shape_creator.hpp"
#include "../core/runtime_environment.hpp"

#include "../io/messenger.hpp"

#include <QFile>
#include <QTextStream>
#include <QString>

#include <string>

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
        
	void dump(const std::string& f) {
            QFile* m_cmdfile = new QFile(f.c_str());
            m_cmdfile->open( QIODevice::WriteOnly | QIODevice::Append ); 
            QTextStream* cmd_stream = new QTextStream(m_cmdfile);
            //(*cmd_stream) << (GET_CMD_ARG(PointListCommandOptionValue,"-points").to_string()+"\n").c_str();
            (*cmd_stream) << (get_cmdname_and_stringified_opts()+"\n").c_str();
            m_cmdfile->flush();
            m_cmdfile->close();
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
