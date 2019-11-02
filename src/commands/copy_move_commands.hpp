#ifndef copy_move_commands_hpp
#define copy_move_commands_hpp

#include "command_manager.hpp"
#include "interactive_command_base.hpp"
#include "direct_command_base.hpp"
#include "selection_commands.hpp"

#include "../core/selection.hpp"
#include "../core/runtime_environment.hpp"
#include "../core/working_set.hpp"
#include "../core/postman.hpp"

#include "../gui/statusbar_manager.hpp"

#include <QApplication>

#include <iostream>
#include <map>

enum relocAction { MOVE, COPY };
namespace {
    std::string relocAction2string(relocAction x) {
            if ( x==MOVE ) return "Move";
            if ( x==COPY ) return "Copy";
    }
}


template<relocAction T>
class dicmdObjRelocateBy : public DirectCommandBase
{
        Selection& m_se = Selection::getInstance();
public:

        dicmdObjRelocateBy(IObjectPoolPtr ptr, QPoint fpos, QPoint tpos)
		: m_ws(std::dynamic_pointer_cast<WorkingSet>(ptr))
	{
                //add_option("-from_region",new PointListCommandOptionValue(pl));
                add_option("-to",new PointCommandOptionValue(tpos));
                add_option("-from",new PointCommandOptionValue(fpos));                
	}

	dicmdObjRelocateBy(IObjectPoolPtr ptr)
		: m_ws(std::dynamic_pointer_cast<WorkingSet>(ptr))
	{
                //add_option("-from_region",new PointListCommandOptionValue(pl));
                add_option("-to",new PointCommandOptionValue());
                add_option("-from",new PointCommandOptionValue());                
	}

	virtual std::string get_name() {
		return "dicmdObjRelocateBy"+relocAction2string(T);
	}

	virtual void execute() override
	{      
                if ( m_se.getObjects().empty() )
                    throw 1;
            //RegionQuery& rq = RegionQuery::getInstance();
                QPoint dst_p(GET_CMD_ARG(PointCommandOptionValue,"-to"));
                QPoint src_p(GET_CMD_ARG(PointCommandOptionValue,"-from"));
                
                for ( auto it : m_se.getObjects() ) {
                    if ( T == COPY )
                        m_ws->addObject(m_se.get_clonee(it));
                    QPoint p((src_p-it->getPoints()[0])+dst_p);
                    m_se.get_clonee(it)->moveCenterToPoint(p);
                    //m_ws->get_clonee(it)->moveCenterToPoint(p);
                }
                
                m_se.clear();
	}

private:
	WorkingSetPtr m_ws = nullptr;
};


template<relocAction T>
class incmdObjRelocateBy : public InteractiveCommandBase  
{
   
        ObjectSandboxPtr m_sb;
        ObjectPoolSandboxPtr m_re;
        IObjectPoolPtr m_ws;
        Selection& m_se = Selection::getInstance();
        command_manager& m_cm = command_manager::getInstance();
        LeCallback* m_sel_cb = nullptr;
public:
	
        incmdObjRelocateBy<T>(ObjectPoolSandboxPtr r, IObjectPoolPtr s ):m_ws(s),m_re(r)
        {
                m_sb = std::shared_ptr<ObjectSandbox>(new ObjectSandbox);
                m_re->addChildren(m_sb);
                //LeCallback cb = 
        }


        virtual void execute() {
                //set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,idle));
                idle(OTHER);
        }

        virtual std::string get_name() {
                return "incmdObjRelocateBy"+relocAction2string(T);
        }

        virtual void abort() {
                //std::cout << "abort!" << std::endl;
                //abort_internal();
        }

//helpers
private:      
        void commit() {
                for ( auto it: m_sb->getPool()->getObjects() ) {
                    RegionQuery& rq = RegionQuery::getInstance();
                    rq.insertObject(m_ws->addObject(it));
                    if ( T == MOVE ) m_ws->removeObject(m_se.get_clonee(m_sb2se[it]));
                }

                 //m_ws->removeObject(dynamic_cast<WorkingSet*>(m_ws.get())->get_clonee(it));
                
                dicmdObjRelocateBy<T>(m_ws,m_clicked_point,InteractiveCommandBase::get_last_point()).log();
                m_se.clear();
                abort_internal();
        }

        void abort_internal() {
                StatusBarManager::getInstance().updateStatusBar("",1,0);
                m_sb->clear();
                //m_se.clear();
                //m_cm.return_to_idle();
                //m_cm.disactivate_active_command();
                m_distances.clear();
                m_sb2se.clear();
                std::cout << "abort2" << std::endl;
        }
    
        void move_runtimes_to_point(QPoint p) {
                for ( auto it: m_sb->getPool()->getObjects() ) {
                    QPoint point(p-m_distances[m_sb2se[it]]);
                    it->moveCenterToPoint(point);
                }
        }
 
 //command cycles
 private:    
        void on_object_selected(LeCallbackData&) {
                m_cm.activate_command(this);
                if (m_sel_cb)
                    m_sel_cb->purge();
                idle(OTHER);
        }
        
        //waiting for selection
        void idle(const EvType& ev) {
                std::cout << "COPYMOVE IDLE" << std::endl;
                // no selection, invoke selectbyregion to select object firsts
                if ( m_se.getObjects().empty() ) {
                    LeCallback cb = REGISTER_CALLBACK(OBJECT_SELECTED,&incmdObjRelocateBy<T>::on_object_selected);
                    if (! m_sel_cb )
                        m_sel_cb = new LeCallback(cb);
                    CommandBase* cmd = m_cm.find_command("incmdSelectShapesByRegion");
                    dynamic_cast<InteractiveCommandBase*>(cmd)->set_auto_repeat(false);
                    m_cm.activate_command(cmd,false);
                    dynamic_cast<InteractiveCommandBase*>(cmd)->set_auto_repeat(true);
                    return;
                }
                
                StatusBarManager::getInstance().updateStatusBar("Click on shape and move mouse to perform action",1,0);
                for ( auto it : m_se.getObjects() ) {
                    IShape* i = m_sb->addObject(it);
                    m_sb2se[i] = it;
                }

                set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,wait_for_first_click));
        }
        
        void wait_for_first_click(const EvType& ev) {
               if ( ev == MC ) {
                    m_clicked_point = InteractiveCommandBase::get_lastclk_point();
                    for ( auto shape: m_se.getObjects() ) {
                        QPoint diff(m_clicked_point - shape->getPoints()[0]);
                        std::cout << m_clicked_point.x() << " " << m_clicked_point.y() << "  shape " << shape << " " << shape->getPoints()[0].x() << " " << shape->getPoints()[0].y()  << " diff "<< diff.x() << " " << diff.y() << std::endl;
                        m_distances[shape] = diff;
                    }
                    set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,act_on_mousemove));
                    StatusBarManager::getInstance().updateStatusBar("Click on desired place to commit action",1,0);
                }
        }
	
        void act_on_mousemove(const EvType& ev) {
                if ( ev == MM )
                    move_runtimes_to_point(InteractiveCommandBase::get_last_point());
                    
                if ( ev == MC ) {
                   
                    if ( ! m_se.getObjects().empty() )
                        on_commit();
                    else
                        m_cm.return_to_idle();
                }
        }

        void on_commit() {
                std::cout << "commit!" << std::endl;
                commit();
        }
        
        QPoint m_clicked_point;
        std::map<IShape*,QPoint> m_distances;
        std::map<IShape*,IShape*> m_sb2se;
};

#endif
