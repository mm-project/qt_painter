#ifndef shape_creator_commands_hpp
#define shape_creator_commands_hpp

#include "command_manager.hpp"
#include "controller.hpp"

#include "interactive_command_base.hpp"

#include <sstream>

class incmdObjCreationBase : public InteractiveCommandBase 
{
    public:
        incmdObjCreationBase(runtime_environment* r,working_set*s):re(r),ws(s) {
            m_controller =  controller::get_instance();
        }

        virtual void handle_update() {
            re->change_basic_properties(m_controller->get_shape_properties());
        }

        void commit() {
            ws->add_object(re->get_runtime_object());
            fini();
        }
        
        void fini() {
            re->reset();
        }
        
        void runtime_set_pos1() {
            re->set_pos1(InteractiveCommandBase::get_last_point());
        }
        
        void runtime_set_pos2() {
            re->set_pos2(InteractiveCommandBase::get_last_point());
        }
        
        runtime_environment* rt() {
            return re;
        }
        
        virtual void abort() {
            //log("dicmdAbortActiveCommand");
            re->reset();
            //FIXME crashing in recursion
            //dicmdAbortActiveCommand d;
            //d.execute_and_log();
            //fini();
            command_manager::get_instance()->return_to_idle();
        }

        
    private:
        runtime_environment* re;
        working_set* ws;
        controller* m_controller; 

};

// produces commands for creating shape by given obj change_object_type
// incmdCreateObj<RECT> => rect
// incmdCreateObj<ELLIPSE> => ellipse

template <ObjectType T>
class incmdCreateObj : public incmdObjCreationBase 
{
    public:
        
        incmdCreateObj(runtime_environment* r, working_set*s ):incmdObjCreationBase(r,s)
        {
            rt()->change_object_type(T);
        }
        
        virtual void execute() {
            set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,idle));
        }
        
        virtual std::string get_name() {
            return "incmdCreateObj"+ObjType2String(T);
        }


    public:      
        void idle(const EvType& ev) {
            //waiting for first mouse click
            if ( ev == KP )
                set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,abort1));
            
            if ( ev != MC )
                return;
            
            runtime_set_pos1();
            set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,on_first_click));
        }
        
        void on_first_click(const EvType& ev) {
            
            if ( ev == KP )
                set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,abort1));
            
            if ( ev == MM )
                runtime_set_pos2();
            else if ( ev == MC || ev == KP )
                set_next_handler(HANDLE_FUNCTION(incmdCreateObj<T>,on_commit));
        }
    
        void on_commit(const EvType&) {
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

    public:
        incmdCreateNthgon(runtime_environment* r, working_set*s ):incmdObjCreationBase(r,s)
        {
            rt()->change_object_type(POLYGON);
            reset_count();
        }
        
        virtual std::string get_name() {
            //FIXME keep stringstream for converting int to str
            std::stringstream z;
            z << "incmdCreateNthgon<" << T << ">";
            return z.str();
        }
        
        virtual void execute() {
            set_next_handler(HANDLE_FUNCTION(incmdCreateNthgon<T>,idle));
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
