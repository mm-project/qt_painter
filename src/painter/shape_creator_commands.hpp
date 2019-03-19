#ifndef shape_creator_commands_hpp
#define shape_creator_commands_hpp

#include "interactive_command_base.hpp"


class obj_creation_base : public interactive_command_base 
{
    public:
        interactive_command_base(runtime_environment* r,working_set*s):re(r),ws(s) {}
        
        void commit() {
            ws->add_object(re->get_runtime_object());
            fini();
        }
        
        void fini() {
            re->reset();
        }
        
        void runtime_set_pos1() {
            re->set_pos1(last);
        }
        
        void runtime_set_pos2() {
            re->set_pos2(last);
        }
        
        runtime_environment* rt() {
            return re;
        }
        
        
    private:
        runtime_environment* re;
        working_set* ws;

};

// produces commands for creating shape by given obj change_object_type
// cmd_create_obj_type<RECT> => rect
// cmd_create_obj_type<ELLIPSE> => ellipse

template <ObjectType T>
class cmd_create_obj_type : public obj_creation_base 
{
    public:
        
        cmd_create_obj_type(runtime_environment* r, working_set*s ):interactive_command_base(r,s)
        {
            rt()->change_object_type(T);
            set_next_step(MEMBER_FUNCTION(cmd_create_obj_type<T>,idle));
        }
        
        void idle(const EvType& ev) {
            //waiting for first mouse click
            if ( ev != MC )
                return;
            
            runtime_set_pos1();
            set_next_step(MEMBER_FUNCTION(cmd_create_obj_type<T>,on_first_click));
        }
        
        void on_first_click(const EvType& ev) {
            if ( ev == MM )
                runtime_set_pos2();
            else if ( ev == MC || ev == KP )
                set_next_step(MEMBER_FUNCTION(cmd_create_obj_type<T>,on_commit));
        }
    
        void on_commit(const EvType&) {
            commit();
            set_next_step(MEMBER_FUNCTION(cmd_create_obj_type<T>,idle));
        }
};



//produces command to create N-angle polygon
// cmd_Nangle_creator_with_sides_num<2> => line
// cmd_Nangle_creator_with_sides_num<3> => triangle
// cmd_Nangle_creator_with_sides_num<6> => hexagon


template<int T>
class cmd_Nangle_creator_with_sides_num : public obj_creation_base 
{
        int count;
    public:
        
        command_Nangle_creator_with_sides_num(runtime_environment* r, working_set*s ):interactive_command_base(r,s)
        {
            rt()->change_object_type(POLYGON);
            set_next_step(MEMBER_FUNCTION(command_Nangle_creator_with_sides_num<T>,idle));
            reset_count();
        }
        
        void reset_count() {
            count = T-1;
        }
        
        void idle(const EvType& ev) {
            //waiting for first mouse click
            std::cout << "interactive command in idle..." << std::endl;
            if ( ev != MC )
                return;
            
            runtime_set_pos1();
            set_next_step(MEMBER_FUNCTION(command_Nangle_creator_with_sides_num<T>,on_first_click));
        }
        
        void on_first_click(const EvType& ev) {
            if ( ev == MC ) {
                std::cout << "interactive command clicked. Remains " << count << " click to commit "std::endl;
                runtime_set_pos1();
                set_next_step(MEMBER_FUNCTION(command_Nangle_creator_with_sides_num<T>,on_first_click));
                if (--count == 0) {
                    //std::cout << "triangle count 0 ..." << std::endl;
                    set_next_step(MEMBER_FUNCTION(command_Nangle_creator_with_sides_num<T>,on_commit));
                }
                    
            }
        }
        
        void on_commit(const EvType&) {
            std::cout << "interactive command COMMIT..." << std::endl;

            commit();
            set_next_step(MEMBER_FUNCTION(command_Nangle_creator_with_sides_num<T>,idle));
            reset_count();
        }
};


#endif
