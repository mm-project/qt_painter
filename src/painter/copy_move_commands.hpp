#ifndef copy_move_commands_hpp
#define copy_move_commands_hpp

/*
class incmdObjCopy : public incmdObjCreationBase 
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
*/

#endif
