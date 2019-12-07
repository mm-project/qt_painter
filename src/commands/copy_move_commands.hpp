#ifndef copy_move_commands_hpp
#define copy_move_commands_hpp

#include "command_manager.hpp"
#include "interactive_command_base.hpp"
#include "direct_command_base.hpp"
#include "selection_commands.hpp"

#include "../core/selection.hpp"
#include "../core/runtime_pool.hpp"
#include "../core/design.hpp"
#include "../core/postman.hpp"

#include "../gui/statusbar_manager.hpp"
#include "../gui/controller.hpp"

#include <QApplication>

#include <iostream>
#include <map>

enum relocAction { MOVE, COPY };
namespace {
    std::string relocAction2string(relocAction x) {
            if ( x==MOVE ) return "Move";
            if ( x==COPY ) return "Copy";
			return "";
    }
}


//dicmdRelocateObjsByMove -selectionbox () -to_point ()

template<relocAction T>
class dicmdObjRelocateBy : public DirectCommandBase
{
        Selection& m_se = Selection::getInstance();
public:

        dicmdObjRelocateBy(ObjectPoolPtr ptr, QPoint fpos, QPoint tpos)
		: m_ws(std::dynamic_pointer_cast<Design>(ptr))
	{
                //add_option("-from_region",new PointListCommandOptionValue(pl));
                add_option("-to",new PointCommandOptionValue(tpos));
                add_option("-from",new PointCommandOptionValue(fpos));                
	}

	dicmdObjRelocateBy(ObjectPoolPtr ptr)
		: m_ws(std::dynamic_pointer_cast<Design>(ptr))
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
					IShapePtr pShape = nullptr;
                    if ( T == COPY )
                        pShape = m_ws->addObject(it);
					else 
						pShape = it;
                    QPoint p((src_p-pShape->getPoints()[0])+dst_p);
                    pShape->moveCenterToPoint(p);
                    //m_ws->get_clonee(it)->moveCenterToPoint(p);
                }
                
                m_se.clear();
	}

    
private:
	DesignPtr m_ws = nullptr;
};


template<relocAction T>
class incmdObjRelocateBy : public InteractiveCommandBase  
{
   
        RuntimePoolPtr m_sb;
        RuntimePoolManagerPtr m_re;
        ObjectPoolPtr m_ws;
        Selection& m_se = Selection::getInstance();
        command_manager& m_cm = command_manager::getInstance();
        LeCallback* m_sel_cb = nullptr;
        bool m_move_move = false;
public:
	
        incmdObjRelocateBy<T>(RuntimePoolManagerPtr r, ObjectPoolPtr s ):m_ws(s),m_re(r)
        {
                //m_sb = std::shared_ptr<ObjectSandbox>(new ObjectSandbox);
                //m_re->addChildren(m_sb);
			m_sb = r->getChild("Generic-InteractiveCommand");
                //LeCallback cb = 
        }

        virtual bool need_log_mouserelease() {
            return true;
			//return m_need_mouserelase_log;
        }

        virtual void execute() {
                //set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,idle));
                m_sb->clear();
                idle(OTHER);
        }

        virtual std::string get_name() {
                return "incmdObjRelocateBy"+relocAction2string(T);
        }

        virtual void abort() {
                //std::cout << "abort!" << std::endl;
                abort_internal();
                m_cm.return_to_idle();
                //std::cout << "abort3" << std::endl;
        }
        
        virtual void handle_update() {
                if ( m_move_move )
                    for ( auto it: m_sb->getObjects() ) {
                        it->updateProperties(controller::getInstance().get_shape_properties());
                        //obj->updateProperties(controller::getInstance().get_shape_properties());        
                    }
        }
//helpers
private:      
        void commit() {
                StatusBarManager::getInstance().updateStatusBar("Commiting...",1,0);
                RegionQuery& rq = RegionQuery::getInstance();
    
                //std::cout << "SELECTION" << m_se.getObjects().size() << "   RTSHAPES: " << m_sb->getPool()->getObjects().size() << "\n";
                //*
				for ( auto it: m_sb->getObjects() ) {
                    rq.insertObject(m_ws->addObject(it));
                    if ( T == MOVE ) {
                        //remove working set's object that has been selected
                        //be fetching from the mapping : runtime_obj -> select_obj 
                        rq.removeObject(m_sb2se[it]);
                        m_ws->removeObject(m_sb2se[it]);
                    }
                }
				/**/
                //dicmdObjRelocateBy<T>(m_ws,m_clicked_point,InteractiveCommandBase::get_last_point()).silent_execute();
                m_se.clear();
                abort_internal();
                //set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,idle));
        }

        void abort_internal() {
                
                m_sb->clear();
                //m_se.clear();
                //m_cm.return_to_idle();
                //m_cm.disactivate_active_command();
                m_distances.clear();
                m_sb2se.clear();
                m_cm.return_to_idle();
                //std::cout << "abort2" << std::endl;
        }
    
        void move_runtimes_to_point(QPoint p) {
                for ( auto it: m_sb->getObjects() ) {
                    QPoint point(p-m_distances[m_sb2se[it]]);
                    it->moveCenterToPoint(point);
                }
        }
 
 //command cycles
 private:    
        void on_object_selected(LeCallbackData&) {
                //m_cm.activate_command(this);
                //if (m_sel_cb)
                //    m_sel_cb->purge();
                //m_sel_cb = nullptr;
                //idle(OTHER);
        }
        
        //waiting for selection
        void idle(const EvType& ev) {
                //std::cout << "COPYMOVE IDLE" <<  m_se.getObjects().size() << std::endl;
                // no selection, invoke selectbyregion to select object firsts and return
                if ( m_se.getObjects().empty() ) {
                    //abort();
                    m_cm.return_to_idle();
                    return;
                    LeCallback cb = REGISTER_CALLBACK(OBJECT_SELECTED,&incmdObjRelocateBy<T>::on_object_selected);
                    if ( m_sel_cb == nullptr )
                        m_sel_cb = new LeCallback(cb);
                    CommandBase* cmd = m_cm.find_command("incmdSelectShapesByRegion");
                    dynamic_cast<InteractiveCommandBase*>(cmd)->set_auto_repeat(false);
                    m_cm.activate_command(cmd,false);
                    dynamic_cast<InteractiveCommandBase*>(cmd)->set_auto_repeat(true);
                    return;
                }

                // shapes finally selected, can copy/move now, moving forward
                int count = m_se.getObjects().size();
                std::string msg("Selected "+QString::number(count).toStdString()+" shapes. Click on shape and move mouse to perform action");
                StatusBarManager::getInstance().updateStatusBar(msg.c_str(),1,0);
                // create runtime objects from selected objects so we can move them with cursor
                // keep mapping runtimeobj<->realobj
                //std::cout << "XXX SELECTION" << m_se.getObjects().size() << "   RTSHAPES: " << m_sb->getPool()->getObjects().size() << "\n";
                for ( auto it : m_se.getObjects() ) {
                    IShapePtr i = m_sb->addObject(it);
                    m_sb2se[i] = it;
                }                                
                //std::cout << "YYY SELECTION" << m_se.getObjects().size() << "   RTSHAPES: " << m_sb->getPool()->getObjects().size() << "\n";


                //wait for user to start the movecopy by clicking.
                set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,wait_for_first_click));
        }
        
        void wait_for_first_click(const EvType& ev) {
               if ( ev == MC || ev == MU || ev == MD  ) {
                    m_clicked_point = InteractiveCommandBase::get_lastclk_point();
                    for ( auto shape: m_se.getObjects() ) {
                        QPoint diff(m_clicked_point - shape->getPoints()[0]);
                        //std::cout << m_clicked_point.x() << " " << m_clicked_point.y() << "  shape " << shape << " " << shape->getPoints()[0].x() << " " << shape->getPoints()[0].y()  << " diff "<< diff.x() << " " << diff.y() << std::endl;
                        m_distances[shape] = diff;
                    }
                    m_move_move = true;
                    set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,act_on_mousemove));
                    StatusBarManager::getInstance().updateStatusBar("Click on desired place to commit action",1,0);
                }
        }
	
        void act_on_mousemove(const EvType& ev) {
                if ( ev == MM )
                    move_runtimes_to_point(InteractiveCommandBase::get_last_point());
                    
                if (  ev == MC || ev == MU || ev == MD  ) {
                   
                    if ( ! m_se.getObjects().empty() )
                        on_commit();
                    else
                        m_cm.return_to_idle();
                }
        }

        void on_commit() {
                //std::cout << "commit!" << std::endl;
                commit();
        }
        
        QPoint m_clicked_point;
        std::map<IShapePtr,QPoint> m_distances;
        std::map<IShapePtr,IShapePtr> m_sb2se;
};

#endif
