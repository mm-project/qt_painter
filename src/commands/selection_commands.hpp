#ifndef selection_commands_hpp
#define selection_commands_hpp

#include "shape_creation_interactive_commands.hpp"

#include "../core/selection.hpp"

#include <QPoint>

#include <cassert>
#include <map>

class incmdSelectUnderCursoer: public InteractiveCommandBase
{
	Selection& m_se = Selection::getInstance();
       
public:

	virtual std::string get_name() {
		return "incmdSelectUnderCursoer";
	}
	
	virtual void execute() {
		InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdSelectUnderCursoer,on_idle));
	}
	
	void on_idle(const EvType& ev) {
		if ( ev == MM )
			m_se.highlight_shape_under_pos(InteractiveCommandBase::get_last_point());
		else if ( ev == MC ) {
			m_se.highlightselect_shape_under_pos(InteractiveCommandBase::get_last_point());
						//m_se->highlight_shape_under_pos(InteractiveCommandBase::get_last_point());
						//on_idle(MM);
				}
	}
	
	virtual void abort() {
	//FIXME now what?
	}
	
	
	virtual void commit() {
	//FIXME now what?
	}
	
				
};


class dicmdSelectShapesByRegion : public DirectCommandBase 
{
        std::pair<QPoint,QPoint> m_reg;

public:
	dicmdSelectShapesByRegion() {
                add_option("-start",new PointCommandOptionValue());
                add_option("-end",new PointCommandOptionValue());
        }
        
	dicmdSelectShapesByRegion(QPoint p1, QPoint p2)  {
                add_option("-start",new PointCommandOptionValue(p1));
                add_option("-end",new PointCommandOptionValue(p2));
                
        }

        virtual void execute() {
                m_reg = std::make_pair<QPoint,QPoint>( GET_CMD_ARG(PointCommandOptionValue,"-start"), GET_CMD_ARG(PointCommandOptionValue,"-end"));
                Selection::getInstance().clear();
                Selection::getInstance().find_and_highlightselect_shapes_from_region(m_reg);
        }
	
        virtual std::string get_name() {
                return "dicmdSelectShapesByRegion";
        }
};


class incmdSelectShapesByRegion : public incmdCreateObj<RECTANGLE>
{
public:
        incmdSelectShapesByRegion(RuntimePoolManagerPtr r, ObjectPoolPtr s ):incmdCreateObj<RECTANGLE>(r,s) {
                m_first_click = true;
        }

        virtual std::string get_name() {
                //FIXME keep stringstream for converting int to str
                return "incmdSelectShapesByRegion";
        }
        
        virtual void execute() {
                
                ObjCreatorCommandBase<RECTANGLE>::set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_idle));
                
                //FIXME how?
                /*
                ShapeProperties sp;
                sp.brush_color = Qt::gray;
                sp.pen_color = Qt::gray;
                sp.pen_style = Qt::DotLine;
                set_properties(sp);
                /**/

        }
       
        virtual void handle_update() {
                //assert(0&&"applying properties to selection box?:)");
        }
        
        virtual void on_commit_internal() {
            //m_se->find_by_range_and_add_to_selected(m_reg);
            dicmdSelectShapesByRegion(m_reg.first,m_reg.second).silent_execute();
            incmdCreateObj<RECTANGLE>::finish();
            ObjCreatorCommandBase<RECTANGLE>::set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_idle));
        }
        
        virtual void handle_mouse_click(int x , int y) {
            if ( m_first_click )
                m_reg.first = InteractiveCommandBase::get_last_point();
            else
                m_reg.second = InteractiveCommandBase::get_last_point();
            
            m_first_click = true;
            InteractiveCommandBase::handle_mouse_click(x,y);
        }
        
//helpers
private:
        std::pair<QPoint,QPoint> m_reg;
        Selection* m_se;
        bool m_first_click;
    
//command cycle
public:
        void on_idle(const EvType& ev) {
            if ( ev == MC ) { 
                if ( m_first_click ) {
                    m_first_click = false;
                    Selection::getInstance().clear();
                }
                
                incmdCreateObj<RECTANGLE>::idle(ev);

                //FIXME doesn't work!!!
                //*
                /*
                if ( incmdCreateObj<RECTANGLE>::idle(ev) ) {
                        //std::cout << "setting.." << std::endl;
                        IShape* s = incmdCreateObj<RECTANGLE>::get_runtime_object();
                        assert(s);
                        
                        ShapeProperties sp;
                        sp.brush_color = Qt::blue;
                        sp.pen_color = Qt::blue;
                        sp.pen_style = Qt::DotLine;
                        
                        //s->updateProperties(sp);
                }
                */
            }
        }
        
};



/*
class incmdSelectShapesByRegion : public ObjCreatorCommandBase<RECTANGLE>
{
	
public:
        incmdSelectShapesByRegion(ObjectPoolSandboxPtr r, IObjectPoolPtr s ):ObjCreatorCommandBase<RECTANGLE>(r,s)
	{
	}
    

        virtual std::string get_name() {
                //FIXME keep stringstream for converting int to str
                return "incmdSelectShapesByRegion";
        }
        
        virtual void execute() {
                m_se = Selection::get_instance();
                m_se->clear();
                ObjCreatorCommandBase<RECTANGLE>::set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_idle));
        }
        
        virtual void abort() {
                //FIXME now what?
        }

        
//helpers
private:
        std::pair<QPoint,QPoint> m_reg;
        Selection* m_se;
    
//command cycle
public:
        void on_idle(const EvType& ev) {
                if ( ev != MC )
                        return;
                
                m_se->clear();
                m_reg.first = InteractiveCommandBase::get_last_point();
                set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_first_click));
        }
        
        void on_first_click(const EvType& ev) {
                if ( ev != MC ) 
                        return;
                
                m_reg.second = InteractiveCommandBase::get_last_point();
                set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_finish));
        }
        
        void on_finish(const EvType&) {
                //std::cout << "interactive command COMMIT..." << std::endl;
                m_se->find_by_range_and_add_to_selected(m_reg);
                set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_idle));
        }
};
*/

#endif
