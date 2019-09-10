#ifndef shape_creator_commands_hpp
#define shape_creator_commands_hpp

#include "command_manager.hpp"
#include "interactive_command_base.hpp"
#include "shape_creation_directive_commands.hpp"

#include "../gui/controller.hpp"
#include "../gui/statusbar_manager.hpp"
#include "../core/shape_creator.hpp"
#include "../core/postman.hpp"


#include <sstream>

template <ObjectType T>
class ObjCreatorCommandBase : public InteractiveCommandBase 
{

	LePostman* m_postman;
	
public:
	ObjCreatorCommandBase<T>(ObjectPoolSandboxPtr r, IObjectPoolPtr s): ws(s) 
	{
		re = std::shared_ptr<ObjectSandbox>(new ObjectSandbox);
		r->addChildren(re);
		m_controller =  controller::get_instance();
		m_rt_shape = 0;
		m_postman = LePostman::get_instance();
	}

	virtual void handle_update() {
		set_properties(m_controller->get_shape_properties());
	}

	//command commits by invoking corresonding non-interactive command
	//and passes gathered points 
	virtual void commit() {
		//begin transaction
		m_internal_vec.push_back(InteractiveCommandBase::get_last_point());
		//m_postman->notify(INTERACTIVE_COMMAND_PRE_COMMIT,a);
		auto ob = re->getPool()->getObjects();
		for (auto i : ob)
		{
			//dicmdCreateObj<T>(m_internal_vec,ws).silent_execute();
			auto cmd = std::shared_ptr<dicmdCreateObj<T>>(new dicmdCreateObj<T>(m_internal_vec, m_controller->get_shape_properties(), ws));
			UndoManager& man = UndoManager::getInstance();
			man.pushCommand(cmd);
			//UndoManager& man = UndoManager::getInstance();
			//man.pushCommand(cmd);
			cmd->silent_execute();
		}
                    //ws->addObject(i);
                //end transaction
		finish();
		//m_postman->notify(INTERACTIVE_COMMAND_POST_COMMIT,a);
	}
	
	virtual void finish() {
		m_internal_vec.clear();
		re->clear();
	}
	
	void set_properties(const ShapeProperties& p) {
		re->changeBasicProperties(p);
	}
        
	IShape* get_runtime_object() {
                return m_rt_shape;
        }
        
	void create_runtime_object() {
                ShapeCreatorPtr shapeCreator = ShapeCreator::getInstance();
		m_rt_shape = shapeCreator->create(T);
                re->addObject(m_rt_shape);
        }
	
	void runtime_set_pos1() {
		re->addPoint(InteractiveCommandBase::get_last_point());
		m_internal_vec.push_back(InteractiveCommandBase::get_last_point());
	}
	
	void runtime_set_pos2() {
		re->addPoint(InteractiveCommandBase::get_last_point());
		if ( T == POLYGON ) 
			m_internal_vec.push_back(InteractiveCommandBase::get_last_point());
	}

	void runtime_movePoint() {
		re->movePoint(InteractiveCommandBase::get_last_point());
		if ( T != POLYGON ) 
			m_internal_vec.push_back(InteractiveCommandBase::get_last_point());
        }
	
	virtual void abort() {
		//log("dicmdAbortActiveCommand");
                finish();
                //m_internal_vec.clear();
		//re->clear();
		//FIXME crashing in recursion
		//dicmdAbortActiveCommand().log();
		//d.execute_and_log();
		//fini();
		command_manager::get_instance()->return_to_idle();
	}

protected:    
	ObjectSandboxPtr re;
private:
	IObjectPoolPtr ws;
	controller* m_controller; 
	IShape* m_rt_shape;
	std::vector<PointCommandOptionValue> m_internal_vec;

};

// produces commands for creating shape by given obj change_object_type
// incmdCreateObj<RECT> => rect
// incmdCreateObj<ELLIPSE> => ellipse
template <ObjectType T>
class incmdCreateObj : public ObjCreatorCommandBase<T>
{
public:
	
	incmdCreateObj(ObjectPoolSandboxPtr r, IObjectPoolPtr s ):ObjCreatorCommandBase<T>(r,s)
	{
	}
	
	virtual void execute() {
		//ObjCreatorCommandBase<T>::create_runtime_object();
                StatusBarManager::getInstance().updateStatusBar("Click and drag on canvas to create shape",1,0);
                InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,idle));
	}
	
	virtual std::string get_name() {
		return "incmdCreateObj"+ObjType2String(T);
	}


public:      
        
	bool idle(const EvType& ev) {

                //std::cout << "idle " << std::endl;
                //waiting for first mouse click
		//assert(0);
		//if ( ev == KP ) //key pressed, abort
		//	InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,abort1));
		
		if ( ev != MC ) //not mouse click, return
			return false;
		
		//mouse clicked , set first point and go to next state 

		ObjCreatorCommandBase<T>::create_runtime_object();
                ObjCreatorCommandBase<T>::handle_update();
                ObjCreatorCommandBase<T>::runtime_set_pos1();
                ObjCreatorCommandBase<T>::runtime_set_pos2();
		InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,on_first_click));
                return true;
        }
	
	void on_first_click(const EvType& ev)
	{
		//assert(0);
		if ( ev == MM )
			ObjCreatorCommandBase<T>::runtime_set_pos2();
		else if ( ev == MC || ev == KP )
                        on_commit(OTHER);
                        //InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,on_commit));

		if ( ev == KP ) //key pressed, abort
			InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,abort1));
	}

	void on_commit(const EvType&) {
		//assert(0);
			StatusBarManager::getInstance().clear();
			on_commit_internal();
	}
	
	//FIXME doesn't work
	void abort1(const EvType&) {
			StatusBarManager::getInstance().clear();
                        ObjCreatorCommandBase<T>::abort();
	}

	virtual void on_commit_internal() {
			StatusBarManager::getInstance().clear();
			ObjCreatorCommandBase<T>::commit();
			InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,idle));
        }
};


template <>
class incmdCreateObj<POLYGON> : public ObjCreatorCommandBase<POLYGON>
{
public:
	incmdCreateObj(ObjectPoolSandboxPtr r, IObjectPoolPtr s ) : ObjCreatorCommandBase<POLYGON>(r,s)
	{
	}

	virtual std::string get_name() {
		return "incmdCreateObj"+ObjType2String(POLYGON);
	}

	virtual void execute()
	{
		InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateObj<POLYGON>, idle));
	}

	bool idle(const EvType& ev) {

		if (ev != MC) //not mouse click, return
			return false;

		ObjCreatorCommandBase<POLYGON>::create_runtime_object();
		ObjCreatorCommandBase<POLYGON>::runtime_set_pos1();
		InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateObj<POLYGON>, on_first_click));
		return true;
	}

	void on_first_click(const EvType& ev)
	{
		//assert(0);
		if ( ev == MC )
			ObjCreatorCommandBase<POLYGON>::runtime_set_pos2();
		else if ( ev == MM)
			ObjCreatorCommandBase<POLYGON>::runtime_movePoint();

		if ( ev == MDC  || ev == KP) //key pressed, abort
			on_commit(OTHER);
                        //InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateObj<POLYGON>,on_commit));
	}
	
	void on_commit(const EvType&) {
		//assert(0);
		on_commit_internal();
	}
	
	//FIXME doesn't work
	void abort1(const EvType&) {
		ObjCreatorCommandBase<POLYGON>::abort();
	}

	virtual void on_commit_internal() {
		ObjCreatorCommandBase<POLYGON>::commit();
		InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateObj<POLYGON>,idle));
	}

};

//produces command to create N-angle polygon
// incmdCreateNthgon<2> => line
// incmdCreateNthgon<3> => triangle
// incmdCreateNthgon<6> => hexagon
template<int T>
class incmdCreateNthgon : public ObjCreatorCommandBase<POLYGON>
{
	int count;
	std::string m_name;

public:
	incmdCreateNthgon(ObjectPoolSandboxPtr r, IObjectPoolPtr s ):ObjCreatorCommandBase<POLYGON>(r,s)
	{
		reset_count();

		std::stringstream z;
		z << "incmdCreateNthgon<" << T << ">";
		m_name = z.str();
	}
	
	virtual std::string get_name() {
		//FIXME keep stringstream for converting int to str
		return m_name;
	}
	
	virtual void execute() {
		InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateNthgon<T>,idle));
	}

	
public:
        
        void reset_count() {
		count = T-1;
	}
	
	void idle(const EvType& ev) {
		//waiting for first mouse click
		//std::cout << "interactive command in idle..." << std::endl;
		if ( ev != MC )
			return;
		
                ObjCreatorCommandBase<POLYGON>::create_runtime_object();
		ObjCreatorCommandBase<POLYGON>::runtime_set_pos1();
		InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateNthgon<T>,on_first_click));
	}
	
	void on_first_click(const EvType& ev) {
		if ( ev == MC ) {
			//std::cout << "interactive command clicked. Remains " << count << " click to commit " << std::endl;
			ObjCreatorCommandBase<POLYGON>::runtime_set_pos1();
			InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateNthgon<T>,on_first_click));
			if (--count == 0) {
				//std::cout << "triangle count 0 ..." << std::endl;
				set_next_handler(HANDLE_FUNCTION(incmdCreateNthgon<T>,on_commit));
			}
				
		}
	}
	
	void on_commit(const EvType&) {
		//std::cout << "interactive command COMMIT..." << std::endl;
		ObjCreatorCommandBase<POLYGON>::commit();
		InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdCreateNthgon<T>,idle));
		reset_count();
	}
};


#endif
