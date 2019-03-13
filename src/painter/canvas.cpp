#include "canvas.hpp"

#include "command.hpp"
#include "command_manager.hpp"
#include "controller.hpp"
#include "shapes.hpp"
#include "working_set.hpp"
#include "runtime_environment.hpp"

#include <QRect>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QMouseEvent>

#include <cassert>
#include <iostream>

canvas::canvas(QWidget* p)
        : QWidget(p), is_runtime_mode(false)
{
        setMouseTracking(true);
        m_working_set = new working_set;
        m_runtime_environment = new runtime_environment();
		//m_working_set = 0;
		//m_active_command = 0;
		m_renderer = new renderer(this);
		cm = command_manager::get_instance();
		cm->init2(m_runtime_environment,m_working_set);
		cm->init();

}

//todo
//void canvas::mouseDoubleClickEven(QMouseEvent* e) {
//
//}

void canvas::mousePressEvent(QMouseEvent* e)
{
		QPoint p(e->pos());

		//if( cm->is_idle() ) return;
		cm->get_active_command()->mouse_clicked(p.x(),p.y());

		// update(); NOTE: only update after commiting object, now is working as object is runtime
		/*
		if (is_runtime_mode) {
                m_runtime_environment->set_pos2(p);
                m_working_set->add_object(m_runtime_environment);
                m_runtime_environment->reset();
                is_runtime_mode = false;
                update();
        } else {
                controller* c = controller::get_instance();
                m_runtime_environment->change_basic_properties(c->get_basic_properties());
                m_runtime_environment->set_pos1(p);
                is_runtime_mode = true;
        }*/
}

void canvas::current_type_changed()
{
        controller* c = controller::get_instance();
        m_runtime_environment->change_object_type(c->get_object_type());
}

void canvas::mouseMoveEvent(QMouseEvent* e)
{
        //if (is_runtime_mode) {
        //        m_runtime_environment->set_pos2(e->pos());
        //
        //}

		//if( cm->is_idle() ) return;
		cm->get_active_command()->mouse_move(e->pos().x(),e->pos().y());
		update();
}



void canvas::wheelEvent(QWheelEvent* event)
{
        //if (is_runtime_mode) {
        //        m_runtime_environment->set_pos2(e->pos());
        //
        //}

		//if( ! m_active_command ) return;
		//m_active_command->mouse_move(e->pos().x(),e->pos().y());
		///dynamic_cast<WPainter>
		//dynamic_cast<WPainter*>(painter)->incr_zoom_factor();
		m_renderer->incr_zoom_factor();
		update();

}

void canvas::paintEvent(QPaintEvent*)
{
		m_renderer->start();
			std::vector<basic_shape*> shapes = m_working_set->get_objects();
			for (auto i = shapes.begin(); i != shapes.end(); ++i) {
					(*i)->draw(m_renderer->get_painter());
			}
			m_runtime_environment->draw_runtime(m_renderer->get_painter());
		m_renderer->stop();
}

void canvas::create_line()
{
        m_runtime_environment->change_object_type(LINE);
}

void canvas::create_rect()
{
		///cm->invoke_command();
		//create_rectangle_command);

		//m_active_command = cm->get_create_rectangle_command();
}

void canvas::create_ellipse()
{
        m_runtime_environment->change_object_type(ELLIPSE);
}

void canvas::create_polygon()
{
        //1. fixme shared
		//m_active_command = cm.get_create_polygon_command();
		//cmd->execute();
        cm->invoke_command();

}

void canvas::reset()
{
        m_working_set->clear();
        update();
}

void canvas::mouseDoubleClickEvent(QMouseEvent* e)
{

    cm->get_active_command()->mouse_dbl_clicked(e->pos().x(),e->pos().y());
    //if( ! m_active_command ) return;
		//m_active_command.try_to_commit();

        //is_runtime_mode = false;
}

