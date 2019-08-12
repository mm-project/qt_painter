#ifndef selection_commands_hpp
#define selection_commands_hpp

#include "shape_creation_interactive_commands.hpp"

#include "../core/selection.hpp"

#include <QPoint>


#include <cassert>

class incmdSelectUnderCursoer: public InteractiveCommandBase
{
        Selection* m_se;
       
	public:
	
		incmdSelectUnderCursoer(ObjectPoolSandboxPtr r, IObjectPoolPtr s ) { //:InteractiveCommandBase(r,s) {
			m_se = Selection::get_instance();
            
		}

        virtual std::string get_name() {
			return "incmdSelectUnderCursoer";
        }
        
        virtual void execute() {
			InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdSelectUnderCursoer,on_idle));
		}
		
		void on_idle(const EvType& ev) {
			if ( ev == MM )
				m_se->select_and_highlight_shape_under_pos(InteractiveCommandBase::get_last_point());
				
		}
		
		virtual void abort() {
        //FIXME now what?
        }
		
		
		virtual void commit() {
        //FIXME now what?
        }
		
				
};

class incmdSelectShapesByRegion : public incmdCreateObj<RECTANGLE>
{
	
public:
        incmdSelectShapesByRegion(ObjectPoolSandboxPtr r, IObjectPoolPtr s ):incmdCreateObj<RECTANGLE>(r,s) {
                m_first_click = true;
                m_se = Selection::get_instance();
            
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
       
       /*
        virtual void abort() {
        //FIXME now what?
        }
        
        //FIXME move to s...
        virtual void commit() {
            //assert(0);
        }
        
        //FIXME move to so...
        virtual void finish() {
            //assert(0);
        }
        */
        
        virtual void on_commit_internal() {
            m_se->find_by_range_and_add_to_selected(m_reg);
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
                    m_se->clear();
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
