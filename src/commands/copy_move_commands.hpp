#ifndef copy_move_commands_hpp
#define copy_move_commands_hpp

#include "command_manager.hpp"
#include "interactive_command_base.hpp"

#include "../core/selection.hpp"
#include "../core/runtime_environment.hpp"
#include "../core/working_set.hpp"

#include "../gui/statusbar_manager.hpp"

#include <iostream>
                
enum relocAction { MOVE, COPY };
namespace {
    std::string relocAction2string(relocAction x) {
            if ( x==MOVE ) return "Move";
            if ( x==COPY ) return "Copy";
    }
}

template<relocAction T>
class incmdObjRelocateBy : public InteractiveCommandBase  
{
   
	ObjectSandboxPtr m_sb;
        ObjectPoolSandboxPtr m_re;
	IObjectPoolPtr m_ws;
        Selection* m_se;
        command_manager* m_cm;
public:
	
	incmdObjRelocateBy<T>(ObjectPoolSandboxPtr r, IObjectPoolPtr s ):m_ws(s),m_re(r)
	{
                m_cm = command_manager::get_instance();
                m_se = Selection::get_instance();
                m_sb = std::shared_ptr<ObjectSandbox>(new ObjectSandbox);
		m_re->addChildren(m_sb);
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
                abort_internal();
        }

//helpers
private:      
        void commit() {
                for ( auto it: m_sb->getPool()->getObjects() ) {
                    m_ws->addObject(it);
                    if ( T == MOVE ) m_ws->removeObject(it); //m_ws->removeObject(dynamic_cast<WorkingSet*>(m_ws.get())->get_clonee(it));
                }
                
                
                m_se->clear();
                abort_internal();
        }

        void abort_internal() {
                StatusBarManager::getInstance().updateStatusBar("",1,0);
                m_sb->clear();
                //m_se->clear();
                //m_cm->return_to_idle();
                //m_cm->disactivate_active_command();
                std::cout << "abort2" << std::endl;
        }
    
        void move_runtimes_to_point(QPoint p) {
                for ( auto it: m_sb->getPool()->getObjects() )
                    it->moveCenterToPoint(p);
        }
 
 //command cycles
 private:    
        
        //waiting for selection
        void idle(const EvType& ev) {
		if ( m_se->getObjects().empty() ) {
                    //abort(); 
                    m_cm->activate_command(m_cm->find_command("incmdSelectShapesByRegion"));
                    //idle(OTHER);
                } else {
                    StatusBarManager::getInstance().updateStatusBar("Click on shape and move mouse to perform action",1,0);
                    for ( auto it : m_se->getObjects() )
                        m_sb->addObject(it);
                    set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,wait_for_first_click));
                }
        }
        
        void wait_for_first_click(const EvType& ev) {
               if ( ev == MC ) {
                    set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,act_on_mousemove));
                    StatusBarManager::getInstance().updateStatusBar("Click on desired place to commit action",1,0);
                }
	}
	
	void act_on_mousemove(const EvType& ev) {
                if ( ev == MM )
                    move_runtimes_to_point(InteractiveCommandBase::get_last_point());
                    
                if ( ev == MC )
                    on_commit();
	}

	void on_commit() {
                std::cout << "commit!" << std::endl;

		commit();
	}
	
};

#endif
