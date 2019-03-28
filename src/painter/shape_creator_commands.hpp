#ifndef shape_creator_commands_hpp
#define shape_creator_commands_hpp

#include "command_manager.hpp"
#include "controller.hpp"

#include "interactive_command_base.hpp"
#include "shape_creator.hpp"

#include <sstream>

class incmdObjCreationBase : public InteractiveCommandBase 
{
public:
	incmdObjCreationBase(ObjectPoolSandboxPtr r, IObjectPoolPtr s): ws(s) 
	{
		re = std::shared_ptr<ObjectSandbox>(new ObjectSandbox);
		r->addChildren(re);
		m_controller =  controller::get_instance();
	}

	virtual void handle_update() {
		//re->change_basic_properties(m_controller->get_shape_properties());
	}

	void commit() {
		auto ob = re->getPool()->getObjects();
		for (auto i : ob)
			ws->addObject(i);
		finish();
	}
	
	void finish() {
		re->clear();
	}
	
	void runtime_set_pos1() {
		re->addPoint(InteractiveCommandBase::get_last_point());
	}
	
	void runtime_set_pos2() {
		re->addPoint(InteractiveCommandBase::get_last_point());
	}
	
	virtual void abort() {
		//log("dicmdAbortActiveCommand");
		re->clear();
		//FIXME crashing in recursion
		//dicmdAbortActiveCommand d;
		//d.execute_and_log();
		//fini();
		command_manager::get_instance()->return_to_idle();
	}

protected:    
	ObjectSandboxPtr re;
private:
	IObjectPoolPtr ws;
	controller* m_controller; 

};

// produces commands for creating shape by given obj change_object_type
// incmdCreateObj<RECT> => rect
// incmdCreateObj<ELLIPSE> => ellipse
template <ObjectType T>
class incmdCreateObj : public incmdObjCreationBase 
{
public:
	
	incmdCreateObj(ObjectPoolSandboxPtr r, IObjectPoolPtr s ):incmdObjCreationBase(r,s)
	{
	}
	
	virtual void execute() {
		set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,idle));
	}
	
	virtual std::string get_name() {
		return "incmdCreateObj"+ObjType2String(T);
	}

	virtual void activate() override
	{
		ShapeCreatorPtr shapeCreator = ShapeCreator::getInstance();
		re->addObject(shapeCreator->create(T));
	}


public:      
	void idle(const EvType& ev) {
		//waiting for first mouse click
		//assert(0);
		if ( ev == KP ) //key pressed, abort
			set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,abort1));
		
		if ( ev != MC ) //not mouse click, return
			return;
		
		//mouse clicked , set first point and go to next state 
		runtime_set_pos1();
		set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,on_first_click));
	}
	
	void on_first_click(const EvType& ev) {
		//assert(0);
		if ( ev == KP ) //key pressed, abort
			set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,abort1));
		
		if ( ev == MM )
			runtime_set_pos2();
		else if ( ev == MC || ev == KP )
			set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,on_commit));
	}

	void on_commit(const EvType&) {
		//assert(0);
		commit();
		set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,idle));
	}
	
	//FIXME doesn't work
	void abort1(const EvType&) {
	   incmdObjCreationBase::abort();
	}

};



//produces command to create N-angle polygon
// incmdCreateNthgon<2> => line
// incmdCreateNthgon<3> => triangle
// incmdCreateNthgon<6> => hexagon
template<int T>
class incmdCreateNthgon : public incmdObjCreationBase 
{
	int count;
	std::string m_name;

public:
	incmdCreateNthgon(ObjectPoolSandboxPtr r, IObjectPoolPtr s ):incmdObjCreationBase(r,s)
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
		set_next_handler(HANDLE_FUNCTION(incmdCreateNthgon<T>,idle));
	}

	virtual void activate() override
	{
		ShapeCreatorPtr shapeCreator = ShapeCreator::getInstacne();
		re->addObject(shapeCreator->create(Polygon));
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
		
		runtime_set_pos1();
		set_next_handler(HANDLE_FUNCTION(incmdCreateNthgon<T>,on_first_click));
	}
	
	void on_first_click(const EvType& ev) {
		if ( ev == MC ) {
			//std::cout << "interactive command clicked. Remains " << count << " click to commit " << std::endl;
			runtime_set_pos1();
			set_next_handler(HANDLE_FUNCTION(incmdCreateNthgon<T>,on_first_click));
			if (--count == 0) {
				//std::cout << "triangle count 0 ..." << std::endl;
				set_next_handler(HANDLE_FUNCTION(incmdCreateNthgon<T>,on_commit));
			}
				
		}
	}
	
	void on_commit(const EvType&) {
		//std::cout << "interactive command COMMIT..." << std::endl;
		commit();
		set_next_handler(HANDLE_FUNCTION(incmdCreateNthgon<T>,idle));
		reset_count();
	}
};


#endif
