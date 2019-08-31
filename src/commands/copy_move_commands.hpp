#ifndef copy_move_commands_hpp
#define copy_move_commands_hpp

#include "command_manager.hpp"
#include "interactive_command_base.hpp"

#include "../core/selection.hpp"
#include "../core/runtime_environment.hpp"
#include "../core/working_set.hpp"

#include "../gui/statusbar_manager.hpp"

                
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
                 set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,idle));
	}
	
	virtual std::string get_name() {
		return "incmdObjRelocateBy"+relocAction2string(T);
	}

        virtual void commit() {
                for ( auto it: m_sb->getPool()->getObjects() ) {
                    m_ws->addObject(it->clone());
                    if ( T == MOVE ) m_ws->removeObject(it);
                }
                    
                abort();
        }

        virtual void abort() {
                //fixme this need to be done in idle :)
                StatusBarManager::getInstance().updateStatusBar("",1,0);
                m_sb->clear();
                m_se->clear();
                m_cm->return_to_idle();
        }

private:      
	
        void move_runtimes_to_point(QPoint p) {
                for ( auto it: m_sb->getPool()->getObjects() )
                    it->moveCenterToPoint(p);
        }
        
        void idle(const EvType& ev) {
		if ( m_se->getObjects().empty() ) 
                    abort(); 
                    //m_cm->activate_command(m_cm->find_command("incmdSelectShapesByRegion"));
                else
                    set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,wait_for_first_click));
        }
        
        void wait_for_first_click(const EvType& ev) {
		StatusBarManager::getInstance().updateStatusBar("Click on shape and move mouse to perform action",1,0);
                if ( ev == MC ) {
                    for ( auto it : m_se->getObjects() )
                    m_sb->addObject(it);
                    set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,act_on_mousemove));
                }

	}
	
	void act_on_mousemove(const EvType& ev) {
                if ( ev == MM )
                    move_runtimes_to_point(InteractiveCommandBase::get_last_point());
                    
                if ( ev == MC )
                    on_commit();
	}

	void on_commit() {
		//assert(0);
		commit();
		//set_next_handler(HANDLE_FUNCTION(incmdObjRelocateBy<T>,idle));
	}
	
};

#endif
