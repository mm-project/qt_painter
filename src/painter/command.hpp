#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "command_manager.hpp"
#include "basic_shape.hpp"
#include "icommand_base.hpp"
#include "controller.hpp"
#include "runtime_environment.hpp"
#include "working_set.hpp"


#include <functional>

class command_idle : public icommand_base 
{
    public:
        void virtual execute_and_log() {} 
        void virtual process() {} 
        void virtual mouse_clicked(int, int) {}
        void virtual mouse_moved(int, int) {} 
        void virtual mouse_dbl_clicked(int, int) {};
        void virtual log() {}
        void virtual update() {}
};

enum EvType { MC, MDC, MM, KP };
#define MEMBER_FUNCTION(C,M) std::bind(&C::M,this,std::placeholders::_1)


typedef std::function<void( const EvType& )> CmdMemFun;

class interactive_command_base : public icommand_base 
{
    public:
        interactive_command_base(runtime_environment* r,working_set*s):re(r),ws(s) {}
        
    public:
        void virtual execute_and_log() {} 
        void virtual process() {} 
        void virtual mouse_clicked(int, int) {m_current_handler(MC);}
        void virtual mouse_moved(int x, int y) {last.setX(x),last.setY(y),m_current_handler(MM);} 
        void virtual mouse_dbl_clicked(int, int) {m_current_handler(MDC);};
        void virtual log() {}
        void virtual update() {}
        
        void set_next_step( CmdMemFun fun ) {
            m_current_handler = fun;
        }
        
        void commit() {
            ws->add_object(re->get_runtime_object());
            fini();
        }
        
        void fini() {
            re->reset();
        }
        
        QPoint get_last_point() {
            return last;
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
        CmdMemFun m_current_handler;
        QPoint last;
        runtime_environment* re;
        working_set* ws;

};

class rect_create : public interactive_command_base 
{
    public:
        
        rect_create(runtime_environment* r, working_set*s ):interactive_command_base(r,s)
        {
            rt()->change_object_type(RECT);
            set_next_step(MEMBER_FUNCTION(rect_create,idle));
        }
        
        void idle(const EvType& ev) {
            //waiting for first mouse click
            if ( ev != MC )
                return;
            
            runtime_set_pos1();
            set_next_step(MEMBER_FUNCTION(rect_create,on_first_click));
        }
        
        void on_first_click(const EvType& ev) {
            if ( ev == MM )
                runtime_set_pos2();
            else if ( ev == MC || ev == KP )
                set_next_step(MEMBER_FUNCTION(rect_create,on_commit));
        }
    
        void on_commit(const EvType&) {
            commit();
            set_next_step(MEMBER_FUNCTION(rect_create,idle));
        }
};


template<int T>
class command_Nangle_creator_with_sides_num : public interactive_command_base 
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
            std::cout << "triangle in idle..." << std::endl;
            if ( ev != MC )
                return;
            
            runtime_set_pos1();
            set_next_step(MEMBER_FUNCTION(command_Nangle_creator_with_sides_num<T>,on_first_click));
        }
        
        void on_first_click(const EvType& ev) {
            std::cout << "triangle clicked ..." << std::endl;

            if ( ev == MC ) {
                runtime_set_pos1();
                set_next_step(MEMBER_FUNCTION(command_Nangle_creator_with_sides_num<T>,on_first_click));
                if (--count == 0) {
                    std::cout << "triangle count 0 ..." << std::endl;
                    set_next_step(MEMBER_FUNCTION(command_Nangle_creator_with_sides_num<T>,on_commit));
                }
                    
            }
        }
        
        void on_commit(const EvType&) {
            std::cout << "triangle COMMIT..." << std::endl;

            commit();
            set_next_step(MEMBER_FUNCTION(command_Nangle_creator_with_sides_num<T>,idle));
            reset_count();
        }
};


template <ObjectType T>
class command_create_shape : public icommand_base
{
    public:
        command_create_shape(runtime_environment* r,working_set* s):re(r),ws(s) {
            m_is_first_click = true;
            m_finished = false;
            re->change_object_type(T);
            command_manager::get_instance()->return_to_idle();
        }
        
        virtual bool isFinished() {
            return m_finished;
        }
        
        virtual void process() {
            m_finished = true;
        }
        
        virtual void mouse_clicked(int x, int y) override {
            if ( m_is_first_click ) {
                m_finished = false;
                controller* c = controller::get_instance();
                re->change_basic_properties(c->get_shape_properties());
                re->set_pos1(QPoint(x,y));
                m_is_first_click = false;
            } else {
                re->set_pos2(QPoint(x,y));
                m_finished = true;
                ws->add_object(re->get_runtime_object());
                re->reset();
                //command_manager::get_instance()->return_to_idle();
                m_is_first_click = true;
            }
            
        }

        virtual void mouse_moved(int x, int y) {
            if ( m_finished ) return;
            re->set_pos2(QPoint(x,y));
        }
        
        virtual void update() {
            re->change_basic_properties(controller::get_instance()->get_shape_properties());
        } 
        
        virtual void mouse_dbl_clicked(int, int) {}

        virtual ~command_create_shape() { }
    //FIXME add accessors so derives can use them
    //private:
        bool m_is_first_click;
        bool m_finished;
        runtime_environment* re;
        working_set* ws;
};


template <>
class command_create_shape<POLYGON> : public command_create_shape<RECT> 
{
    public:
        command_create_shape(runtime_environment* r,working_set*s):command_create_shape<RECT>(r,s)
        {
            re->change_object_type(POLYGON);
        }

    public:
        void mouse_clicked(int x, int y)
        {
            if ( m_is_first_click ) {
                m_finished = false;
                controller* c = controller::get_instance();
                re->change_basic_properties(c->get_shape_properties());
                re->set_pos1(QPoint(x,y));
            }
        }

        void  mouse_dbl_clicked(int x, int y) 
        {
            //assert(0);
            re->set_pos2(QPoint(x,y));
            m_finished = true;
            controller* c = controller::get_instance();
            re->change_basic_properties(c->get_shape_properties());
            ws->add_object(re->get_runtime_object());
            re->reset();
            //command_manager::get_instance()->return_to_idle();
            m_is_first_click = true;    
        }

        void  mouse_moved(int x, int y)
        {
            if ( m_finished ) 
                return;
            
            re->set_pos2(QPoint(x,y));
        }
        
        
        virtual ~command_create_shape() { }

};


#endif
