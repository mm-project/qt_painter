#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "command_manager.hpp"
#include "basic_shape.hpp"
#include "icommand_base.hpp"
#include "controller.hpp"
#include "runtime_environment.hpp"
#include "working_set.hpp"


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
