#ifndef selection_commands_hpp
#define selection_commands_hpp

#include "shape_creation_interactive_commands.hpp"

#include "../core/selection.hpp"
#include "../core/postman.hpp"
#include "../core/runtime_environment.hpp"
#include "../gui/statusbar_manager.hpp"

#include <QPoint>

#include <cassert>
#include <map>
#include <iostream>

class incmdSelectUnderCursoer: public InteractiveCommandBase
{
        Selection* m_se;
        bool m_move_mode = false;
        ObjectSandboxPtr m_sb;
        ObjectPoolSandboxPtr m_re;
        IObjectPoolPtr m_ws;
        command_manager* m_cm;
        LeCallback* m_sel_cb;

public:
	
        incmdSelectUnderCursoer(ObjectPoolSandboxPtr r, IObjectPoolPtr s ):m_ws(s),m_re(r) {
			m_se = Selection::get_instance();
            m_sb = std::shared_ptr<ObjectSandbox>(new ObjectSandbox);
            m_re->addChildren(m_sb);

		}

		virtual void abort() {
        //FIXME now what?
        }

        virtual std::string get_name() {
			return "incmdSelectUnderCursoer";
        }
        
        virtual void execute() {
			InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdSelectUnderCursoer,on_idle));
		}
		
		void on_idle(const EvType& ev) {
			if ( ev == MM )
                    if ( ! m_move_mode ) 
                        m_se->highlight_shape_under_pos(InteractiveCommandBase::get_last_point());
                    else 
                         move_selected_to_point(InteractiveCommandBase::get_last_point());
            else if ( ev == MC ) 
                   on_click();
            else if ( ev == MR )
                    m_move_mode=false;
		}
		
    private:
        void on_click() {
             m_move_mode=true;
             m_se->highlightselect_shape_under_pos(InteractiveCommandBase::get_last_point());
             for ( auto it : m_se->getObjects() )
                m_sb->addObject(it);

        }
        
        void move_selected_to_point(QPoint p) {
            for ( auto it: m_sb->getPool()->getObjects() ) {
                    std::cout << "rotate..." << std::endl; 
                    it->moveCenterToPoint(p);
            }
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
                Selection::get_instance()->clear();
                Selection::get_instance()->find_and_highlightselect_shapes_from_region(m_reg);
                if ( ! Selection::get_instance()->getObjects().empty() ) {
                    LeCallbackData d;
                    NOTIFY(OBJECT_SELECTED,d);
                }
        }
	
        virtual std::string get_name() {
                return "dicmdSelectShapesByRegion";
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
                StatusBarManager::getInstance().updateStatusBar("Click to select by region",1,0);

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
                std::cout << "hu" << std::endl;
                //assert(0&&"applying properties to selection box?:)");
        }
        
        virtual void on_commit_internal() {
            //m_se->find_by_range_and_add_to_selected(m_reg);
            dicmdSelectShapesByRegion(m_reg.first,m_reg.second).silent_execute();
            incmdCreateObj<RECTANGLE>::finish();
            if ( InteractiveCommandBase::is_auto_repeating() )
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
