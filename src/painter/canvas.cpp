#include "canvas.hpp"

#include "direct_command_base.hpp"
#include "basic_commands.hpp"
#include "gui_commands.hpp"
#include "shape_creator_commands.hpp"
#include "selection_commands.hpp"
#include "command_manager.hpp"
#include "controller.hpp"
#include "shapes.hpp"
#include "working_set.hpp"
#include "runtime_environment.hpp"
#include "selection.hpp"

#include <QRect>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QMouseEvent>

#include <cassert>
#include <iostream>

#define INCMD_CREATE_OBJ(S) incmdCreateObj<S>(m_sandbox, m_working_set)
#define INCMD_CREATE_OBJ_POLYGON(N) incmdCreateNthgon<N>(m_sandbox, m_working_set)
#define INCMD_HIGHLIGHT_BY_REGION incmdSelectShapesByRegion(m_sandbox, m_working_set)



canvas::canvas(QWidget* p)
        : QWidget(p), is_runtime_mode(false)
{
        setFocusPolicy(Qt::StrongFocus);
        setMouseTracking(true);
        setObjectName("CANVAS");
        
        //FIXME move to services
        m_working_set = std::shared_ptr<WorkingSet>(new WorkingSet);
        m_sandbox = std::shared_ptr<ObjectPoolSandbox>(new ObjectPoolSandbox);
        Selection::get_instance()->set_working_set(m_working_set.get());
        Selection::get_instance()->set_sandbox(m_sandbox.get());
        
        
        m_renderer = new renderer(this);
        cm = command_manager::get_instance();
        cm->init2(m_sandbox, m_working_set);
        cm->init();
        
        //FIXME broken
        cm->register_command(new INCMD_CREATE_OBJ(LINE));
        cm->register_command(new INCMD_CREATE_OBJ(RECTANGLE));
        cm->register_command(new INCMD_CREATE_OBJ(ELLIPSE));
        cm->register_command(new INCMD_CREATE_OBJ(POLYGON));
        cm->register_command(new INCMD_HIGHLIGHT_BY_REGION);

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
    //dicmdCanvasMouseMove(e->pos()).log();

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

	// black hole :D
    QRect rect(QPoint(0, 0), QSize(1000,1000));
    QBrush b(Qt::black);
    painter->setBrush(b);
    painter->drawRect(rect);

	// draw working set
    std::vector<IShape*> shapes = m_working_set->getObjects();
    for (auto i : shapes)
                    i->draw(painter);
   
    // draw runtime
    auto pools = m_sandbox->getChildren();
    for (auto it : pools)
    {
            if (it == nullptr)
                    continue;

            auto p = it->getPool();
            if (p == nullptr)
                    continue;

            auto objs = p->getObjects();
            for (auto i : objs)
                    i->draw(painter);
    }

    m_renderer->stop();
}

void canvas::invoke_create_line()
{
    cm->activate_command(cm->find_command("incmdCreateObjLine"));
    //cm->activate_command(new INCMD_CREATE_OBJ(LINE));
}

void canvas::invoke_create_rect()
{
    cm->activate_command(cm->find_command("incmdCreateObjRectangle"));
    //cm->activate_command(new INCMD_CREATE_OBJ(RECT));
}

void canvas::invoke_create_ellipse()
{
    cm->activate_command(cm->find_command("incmdCreateObjEllipse"));
    //cm->activate_command(new INCMD_CREATE_OBJ(ELLIPSE));
}

void canvas::invoke_create_polygon()
{
    cm->activate_command(cm->find_command("incmdCreateObjPolygon"));
   //cm->activate_command(new INCMD_CREATE_OBJ_POLYGON(3));
}

void canvas::invoke_select_by_region()
{
    cm->activate_command(cm->find_command("incmdSelectShapesByRegion"));
   //cm->activate_command(new INCMD_CREATE_OBJ_POLYGON(3));
}


void canvas::reset()
{
    m_working_set->clear();
    update();
}
