#ifndef shape_creator_commands_hpp
#define shape_creator_commands_hpp


#include "interactive_command_base.hpp"


class incmdObjCreationBase : public InteractiveCommandBase 
{
    public:
        incmdObjCreationBase(runtime_environment* r,working_set*s):re(r),ws(s) {}

        virtual void handle_update() {
            //change runtime attributes
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
        
        
        
    private:
        runtime_environment* re;
        working_set* ws;

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
            set_next_step(MEMBER_FUNCTION(incmdCreateObj<T>,idle));
        }
        
        void idle(const EvType& ev) {
            //waiting for first mouse click
            if ( ev != MC )
                return;
            
            runtime_set_pos1();
            set_next_step(MEMBER_FUNCTION(incmdCreateObj<T>,on_first_click));
        }
        
        void on_first_click(const EvType& ev) {
            if ( ev == MM )
                runtime_set_pos2();
            else if ( ev == MC || ev == KP )
                set_next_step(MEMBER_FUNCTION(incmdCreateObj<T>,on_commit));
        }
    
        void on_commit(const EvType&) {
            commit();
            set_next_step(MEMBER_FUNCTION(incmdCreateObj<T>,idle));
        }
        
        virtual std::string get_name() {
            return "incmdCreateObj"+ObjType2String(T);
        }
        
        
    
};



//produces command to create N-angle polygon
// cmd_Nangle_creator_with_sides_num<2> => line
// cmd_Nangle_creator_with_sides_num<3> => triangle
// cmd_Nangle_creator_with_sides_num<6> => hexagon


template<int T>
class incmdCreateNthgon : public incmdObjCreationBase 
{
        int count;
    public:
        
        virtual std::string get_name() {
            return "incmdCreateNthgon+Nfixme";
        }
        
        virtual void execute() {
            set_next_step(MEMBER_FUNCTION(incmdCreateNthgon<T>,idle));
        }

        incmdCreateNthgon(runtime_environment* r, working_set*s ):incmdObjCreationBase(r,s)
        {
            rt()->change_object_type(POLYGON);
            reset_count();
        }
        
        void reset_count() {
            count = T-1;
        }
        
        void idle(const EvType& ev) {
            //waiting for first mouse click
            //std::cout << "interactive command in idle..." << std::endl;
            if ( ev != MC )
                return;
            
            runtime_set_pos1();
            set_next_step(MEMBER_FUNCTION(incmdCreateNthgon<T>,on_first_click));
        }
        
        void on_first_click(const EvType& ev) {
            if ( ev == MC ) {
                //std::cout << "interactive command clicked. Remains " << count << " click to commit " << std::endl;
                runtime_set_pos1();
                set_next_step(MEMBER_FUNCTION(incmdCreateNthgon<T>,on_first_click));
                if (--count == 0) {
                    //std::cout << "triangle count 0 ..." << std::endl;
                    set_next_step(MEMBER_FUNCTION(incmdCreateNthgon<T>,on_commit));
                }
                    
            }
        }
        
        void on_commit(const EvType&) {
            //std::cout << "interactive command COMMIT..." << std::endl;

            commit();
            set_next_step(MEMBER_FUNCTION(incmdCreateNthgon<T>,idle));
            reset_count();
        }
};


#endif
