#ifndef selection_commands_hpp
#define selection_commands_hpp

#include "shape_creator_commands.hpp"
#include "selection.hpp"

#include <QPoint>


#include <cassert>

class incmdSelectShapesByRegion : public incmdCreateObj<RECTANGLE>
{
	
public:
        incmdSelectShapesByRegion(ObjectPoolSandboxPtr r, IObjectPoolPtr s ):incmdCreateObj<RECTANGLE>(r,s)
	{
                m_first_click = true;
                m_se = Selection::get_instance();

	}

        virtual std::string get_name() {
                //FIXME keep stringstream for converting int to str
                return "incmdSelectShapesByRegion";
        }
        
        virtual void execute() {
                m_se->clear();
                //incmdCreateObj<RECTANGLE>::execute();
                ObjCreatorCommandBase<RECTANGLE>::set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_idle));
        }
        
        virtual void abort() {
                //FIXME now what?
        }
        
        virtual void commit() {
           
        }
        
        virtual void finish() {
            m_se->find_by_range_and_add_to_selected(m_reg);
            incmdCreateObj<RECTANGLE>::finish();
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
                if ( incmdCreateObj<RECTANGLE>::idle(ev) ) {
                    IShape* s = incmdCreateObj<RECTANGLE>::get_runtime_object();
                    assert(s);
                    
                    ShapeProperties sp;
                    sp.brush_color = Qt::red;
                    sp.pen_color = Qt::red;
                    sp.pen_style = Qt::DotLine;
                    
                    s->updateProperties(sp);
                }
        }
        
};



/*
class incmdSelectShapesByRegion : public ObjCreatorCommandBase<RECTANGLE>
{
	
	incmdSelectShapesByRegion(ObjectPoolSandboxPtr r, IObjectPoolPtr s ):ObjCreatorCommandBase<RECTANGLE>(r,s)
	{
	}
    
public:
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
