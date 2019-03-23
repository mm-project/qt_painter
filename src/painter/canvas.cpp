#include "canvas.hpp"

#include "direct_command_base.hpp"
#include "basic_commands.hpp"
#include "shape_creator_commands.hpp"
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

#define INCMD_CREATE_OBJ(S) incmdCreateObj<S>(m_runtime_environment,m_working_set)
#define INCMD_CREATE_OBJ_POLYGON(N) incmdCreateNthgon<N>(m_runtime_environment,m_working_set)


canvas::canvas(QWidget* p)
        : QWidget(p), is_runtime_mode(false)
{
        setFocusPolicy(Qt::StrongFocus);
        setMouseTracking(true);
        setObjectName("CANVAS");
        
        m_working_set = new working_set;
        m_runtime_environment = new runtime_environment();
        m_renderer = new renderer(this);
        cm = command_manager::get_instance();
        cm->init2(m_runtime_environment,m_working_set);
        cm->init();
        
        //FIXME broken
        //cm->register_command(new INCMD_CREATE_OBJ(LINE));
        cm->register_command(new INCMD_CREATE_OBJ(RECT));
        //cm->register_command(new INCMD_CREATE_OBJ(ELLIPSE));
        //cm->register_command(new INCMD_CREATE_OBJ(POLYGON));

}

void canvas::keyPressEvent(QKeyEvent*) {
    //FIXME some interface for keys needed
    //assert(0);

    if( cm->is_idle() ) 
        return;

    //if key pressed esc
    //command_manager::get_instance()->get_active_command()->abort();
    //cm->key_pressed();
}

void canvas::mousePressEvent(QMouseEvent* e)
{
    if( cm->is_idle() ) 
        return;

    QPoint p(e->pos());
    dicmdCanvasMouseClick(p).log();
    cm->mouse_clicked(p.x(),p.y());
}

//FIXME not needed anymore
void canvas::current_type_changed()
{
    //not needed
    //controller* c = controller::get_instance();
    //m_runtime_environment->change_object_type(c->get_object_type());
}

void canvas::mouseMoveEvent(QMouseEvent* e)
{
    if( cm->is_idle() ) 
        return;
    
    cm->mouse_moved(e->pos().x(),e->pos().y());
    //FIXME add logMotion flag to enable
    dicmdCanvasMouseMove(e->pos()).log();

    update();
}

void canvas::wheelEvent(QWheelEvent*)
{
    m_renderer->incr_zoom_factor();
    update();
}

void canvas::mouseDoubleClickEvent(QMouseEvent* e)
{
    cm->mouse_dbl_clicked(e->pos().x(),e->pos().y());
    update();
}

void canvas::on_update()
{
    cm->update();
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

void canvas::invoke_create_line()
{
    //cm->activate_command(cm->find_command("incmdCreateObjLine"));
    cm->activate_command(new INCMD_CREATE_OBJ(LINE));
}

void canvas::invoke_create_rect()
{
    //cm->activate_command(cm->find_command("incmdCreateObjRectangle"));
    cm->activate_command(new INCMD_CREATE_OBJ(RECT));
}

void canvas::invoke_create_ellipse()
{
    //cm->activate_command(cm->find_command("incmdCreateObjEllipse"));
    cm->activate_command(new INCMD_CREATE_OBJ(ELLIPSE));
}

void canvas::invoke_create_polygon()
{
    //cm->activate_command(cm->find_command("incmdCreateObjPolygon"));
   cm->activate_command(new INCMD_CREATE_OBJ_POLYGON(3));
}

void canvas::reset()
{
    m_working_set->clear();
    update();
}
