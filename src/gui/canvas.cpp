#include "canvas.hpp"
#include "controller.hpp"

#include "../core/shapes.hpp"
#include "../core/working_set.hpp"
#include "../core/runtime_environment.hpp"
#include "../core/selection.hpp"

#include "../commands/direct_command_base.hpp"
#include "../commands/basic_commands.hpp"
#include "../commands/gui_commands.hpp"
#include "../commands/shape_creator_commands.hpp"
#include "../commands/selection_commands.hpp"
#include "../commands/command_manager.hpp"


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
#define INCMD_HIGHLIGHT_BY_POINT incmdSelectUnderCursoer(m_sandbox, m_working_set)


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
	
	m_renderer = new renderer(this,m_sandbox,m_working_set);
	
	cm = command_manager::get_instance();
	cm->init2(m_sandbox, m_working_set);
	cm->init();
	
	//fixme to other place
	cm->register_command(new INCMD_CREATE_OBJ(LINE));
	cm->register_command(new INCMD_CREATE_OBJ(RECTANGLE));
	cm->register_command(new INCMD_CREATE_OBJ(ELLIPSE));
	cm->register_command(new INCMD_CREATE_OBJ(POLYGON));
	cm->register_command(new INCMD_HIGHLIGHT_BY_REGION);
	cm->register_command(new INCMD_HIGHLIGHT_BY_POINT);
}

void canvas::keyPressEvent(QKeyEvent*) {
    if( cm->is_idle() ) 
        return;
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

	
	int _x = e->pos().x();
	int _y = e->pos().y();
	//_x = (_x / m_scale) * m_scale;
	//_y = (_y / m_scale) * m_scale;
	//e->pos().setX(_x);
	//e->pos().setY(_y);
    cm->mouse_moved(_x, _y);
    //FIXME add logMotion flag to enable
    //dicmdCanvasMouseMove(e->pos()).log();
	/**/
	
    update();
}

void canvas::wheelEvent(QWheelEvent* pEvent)
{
	m_renderer->zoom((pEvent->delta()/120));
	
	/*
	QPoint numDegrees = pEvent->angleDelta() / 8;
	
	int zoom = 0;
	if (numDegrees.y() < 0)
	{
		m_renderer->incr_zoom_factor();
		while (m_renderer->get_zoom_factor() < 2)
			m_renderer->incr_zoom_factor();
		zoom = m_renderer->get_zoom_factor();
		if (zoom < 10)
			m_scale *= zoom;
	}
	else if (numDegrees.y() > 0)
	{
		zoom = m_renderer->get_zoom_factor();
		m_renderer->decr_zoom_factor();
		if (zoom > 0)
			m_scale /= zoom;
	}
	if (m_scale < 20)
		m_scale = 20;

	std::cout << m_scale << std::endl;
	*/
    update();
}

void canvas::mouseDoubleClickEvent(QMouseEvent* e)
{
    dicmdCanvasMouseDblClick(e->pos()).log();
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
 	m_renderer->render();
	update();
}

void canvas::invoke_create_line()
{
    cm->activate_command(cm->find_command("incmdCreateObjLine"));
}

void canvas::invoke_create_rect()
{
    cm->activate_command(cm->find_command("incmdCreateObjRectangle"));
}

void canvas::invoke_create_ellipse()
{
    cm->activate_command(cm->find_command("incmdCreateObjEllipse"));
}

void canvas::invoke_create_polygon()
{
    cm->activate_command(cm->find_command("incmdCreateObjPolygon"));
}

void canvas::invoke_select_by_region()
{
    cm->activate_command(cm->find_command("incmdSelectShapesByRegion"));
}

void canvas::invoke_select_by_point()
{
    cm->activate_command(cm->find_command("incmdSelectUnderCursoer"));
}

void canvas::reset()
{
    m_working_set->clear();
    update();
}
