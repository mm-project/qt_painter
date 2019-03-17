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

void canvas::mousePressEvent(QMouseEvent* e)
{
    QPoint p(e->pos());

    if( cm->is_idle() ) return;
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
    //not needed
    //controller* c = controller::get_instance();
    //m_runtime_environment->change_object_type(c->get_object_type());
}

void canvas::mouseMoveEvent(QMouseEvent* e)
{
    if( cm->is_idle() ) return;
    cm->get_active_command()->mouse_move(e->pos().x(),e->pos().y());
    update();
}

void canvas::wheelEvent(QWheelEvent* event)
{
    m_renderer->incr_zoom_factor();
    update();
}

void canvas::paintEvent(QPaintEvent*)
{
    auto painter = m_renderer->get_painter();
    m_renderer->start();
    QRect rect(QPoint(0, 0), QSize(1000,1000));
    QBrush b(Qt::black);
    painter->setBrush(b);
    painter->drawRect(rect);
    std::vector<IBasicShape*> shapes = m_working_set->get_objects();
    for (auto i = shapes.begin(); i != shapes.end(); ++i) {
                    (*i)->draw(painter);
    }
    m_runtime_environment->draw_runtime(painter);
    m_renderer->stop();
}

void canvas::create_line()
{
    cm->activate_command(new command_create_shape<LINE>(m_runtime_environment,m_working_set));
}

void canvas::create_rect()
{
    cm->activate_command(new command_create_shape<RECT>(m_runtime_environment,m_working_set));
}

void canvas::create_ellipse()
{
   cm->activate_command(new command_create_shape<ELLIPSE>(m_runtime_environment,m_working_set));
}

void canvas::create_polygon()
{
   cm->activate_command(new command_create_shape<RECT>(m_runtime_environment,m_working_set));
}

void canvas::reset()
{
    m_working_set->clear();
    update();
}

void canvas::mouseDoubleClickEvent(QMouseEvent* e)
{
    cm->get_active_command()->mouse_dbl_clicked(e->pos().x(),e->pos().y());
}

