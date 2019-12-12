#include "canvas.hpp"
#include "controller.hpp"

#ifdef NO_RQ
    #include "../core/rq/RegionQueryService.hpp"
#endif

#include "../core/shapes.hpp"
#include "../core/design.hpp"
#include "../core/runtime_pool.hpp"
#include "../core/selection.hpp"
#include "../core/application.hpp"

#include "../commands/direct_command_base.hpp"
#include "../commands/basic_commands.hpp"
#include "../commands/gui_commands.hpp"
#include "../commands/shape_creation_interactive_commands.hpp"
#include "../commands/shape_creation_directive_commands.hpp"
#include "../commands/selection_commands.hpp"
#include "../commands/load_save_commands.hpp"
#include "../commands/interactive_load_save.hpp"
#include "../commands/delete_command.hpp"
#include "../commands/copy_move_commands.hpp"
#include "../commands/command_manager.hpp"

#include <QRect>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QMouseEvent>

#include <cassert>
#include <iostream>

#define INCMD_CREATE_OBJ(S) incmdCreateObj<S>(m_runtime, m_design)
#define INCMD_CREATE_OBJ_POLYGON(N) incmdCreateNthgon<N>(m_runtime, m_design)
#define INCMD_HIGHLIGHT_BY_REGION incmdSelectShapesByRegion(m_runtime, m_design)
#define INCMD_HIGHLIGHT_BY_POINT incmdSelectUnderCursoer(m_runtime, m_design)

canvas::canvas(QWidget* p)
        : QWidget(p), is_runtime_mode(false)
{
	setFocusPolicy(Qt::StrongFocus);
	setMouseTracking(true);
	setObjectName("CANVAS");
        //setStyleSheet("background-color:black;");
	
        //fixme need preferences
    m_need_motionlog = !QString::fromLocal8Bit( qgetenv("PAINTER_LOG_MOTION").constData() ).isEmpty();
        
	//FIXME move to services
	m_design = std::shared_ptr<Design>(new Design);
	m_runtime = std::shared_ptr<RuntimePoolManager>(&RuntimePoolManager::getInstance());
	//	Global runtime pool
	auto runtimePool = std::shared_ptr<RuntimePool>(new RuntimePool);
	m_runtime->addChild(runtimePool, "Generic-InteractiveCommand");
		
	
	Selection::getInstance().set_working_set(m_design);
	Selection::getInstance().set_sandbox(m_runtime);
#ifdef NO_RQ
    RegionQuery::getInstance().setWS(m_working_set);
#endif
	
	m_renderer = new renderer(this, m_runtime, m_design);
	
	cm.init2(m_runtime, m_design);
	cm.init();
    cm.set_main_renderer(m_renderer);
	
	//fixme move to other place
    cm.register_command(new INCMD_CREATE_OBJ(LINE));
    cm.register_command(new INCMD_CREATE_OBJ(RECTANGLE));
    cm.register_command(new INCMD_CREATE_OBJ(ELLIPSE));
    cm.register_command(new INCMD_CREATE_OBJ(POLYGON));
    cm.register_command(new INCMD_HIGHLIGHT_BY_REGION);
    cm.register_command(new INCMD_HIGHLIGHT_BY_POINT);
    cm.register_command(new dicmdCreateObj<RECTANGLE>(m_design));
    cm.register_command(new dicmdCreateObj<LINE>(m_design));
    cm.register_command(new dicmdCreateObj<ELLIPSE>(m_design));
    cm.register_command(new dicmdCreateObj<POLYGON>(m_design));
    cm.register_command(new InteractiveDesAction<LOAD>(m_design));
    cm.register_command(new InteractiveDesAction<SAVE>(m_design));
    cm.register_command(new InteractiveDesAction<NEW>(m_design));
    cm.register_command(new dicmdDesignSave(m_design));
    cm.register_command(new dicmdDesignLoad(m_design));
    cm.register_command(new InteractiveDeleteAction(m_design));
    cm.register_command(new dicmdDeleteObj(m_design));
    cm.register_command(new dicmdObjRelocateBy<MOVE>(m_design));
    cm.register_command(new dicmdObjRelocateBy<COPY>(m_design));
    cm.register_command(new incmdObjRelocateBy<MOVE>(m_runtime, m_design));
    cm.register_command(new incmdObjRelocateBy<COPY>(m_runtime, m_design));
    cm.set_idle_command(cm.find_command("incmdSelectUnderCursoer"));
    //cm.set_idle_command(new INCMD_HIGHLIGHT_BY_POINT);
  }



renderer* canvas::get_renderer() {
    return m_renderer;
}
 
//this is temporary 
bool canvas::event(QEvent* event) 
{
    if (event->type() == QEvent::User ) {
            QPoint p = (dynamic_cast<QMouseEvent*>(event))->pos();
            cm.mouse_pressed(p.x(),p.y());
            update();
    }
   return QWidget::event(event);
}

void canvas::keyPressEvent(QKeyEvent* ev) {
    
    //binding goes here
    //if(ev->modifiers() & Qt::ShiftModifier) {
        //if ( ev->key() == Qt::Key_1 )  cm.find_command("dicmdQaCompareCanvas")->execute();
        //better handling
        if ( ev->key() == Qt::Key_M )  
             cm.activate_command(cm.find_command("incmdObjRelocateByMove"));
        else if ( ev->key() == Qt::Key_C )  
             cm.activate_command(cm.find_command("incmdObjRelocateByCopy"));        
        else if ( ev->key() == Qt::Key_2 ) 
            cm.find_command("dicmdQaCompareSelection")->execute_and_log();
        else if ( ev->key() == Qt::Key_1 )
            m_renderer->rendering_mode_change();
        else if ( ev->key() == Qt::Key_4 ) 
            cm.find_command("dicmdQaCompareRuntime")->execute_and_log();
        else if ( ev->key() == Qt::Key_3 )
            m_renderer->rendering_rt_mode_change();
        else if ( ev->key() == Qt::Key_Z ) 
            m_renderer->zoomout_p(m_last_cursor);
        else if ( ev->key() == Qt::Key_X ) 
            m_renderer->zoomin_p(m_last_cursor); 
        else if ( ev->key() == Qt::Key_Up )
            m_renderer->pan(PANUP);
        else if ( ev->key() == Qt::Key_Down )
            m_renderer->pan(PANDOWN);
        else if ( ev->key() == Qt::Key_Left )
            m_renderer->pan(PANLEFT);
        else if ( ev->key() == Qt::Key_Right )
            m_renderer->pan(PANRIGHT);
        else if ( ev->key() == Qt::Key_Escape )
           cm.disactivate_active_command();
        else if ( ev->key() == Qt::Key_Q )
           cm.find_command("dicmdQaReplyingBreak")->execute_and_log();
        else if ( ev->key() == Qt::Key_W )
           cm.find_command("dicmdQaReplyingResume")->execute_and_log();
        else if ( ev->key() == Qt::Key_S )
            cm.activate_command(cm.find_command("incmdSelectShapesByRegion"));
        else if ( ev->key() == Qt::Key_N )
           cm.find_command("dicmdQaReplyStep")->execute_and_log();
        else {
            if( cm.is_idle() ) 
                return;
            cm.key_pressed();
        }       
        update();        
}

void canvas::mousePressEvent(QMouseEvent* e)
{
    if( cm.is_idle() ) 
        return;

    QPoint p(e->pos());
    //if(!Application::is_replay_mode())
    //if(!Application::is_log_mode())
    //dicmdCanvasMouseClick(p).log();
    cm.mouse_clicked(p.x(),p.y());
    m_renderer->click_hint();
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
    m_last_cursor = e->pos();
    if( cm.is_idle() ) 
        return;

	int _x = e->pos().x();
	int _y = e->pos().y();
	//_x = (_x / m_scale) * m_scale;
	//_y = (_y / m_scale) * m_scale;
	//e->pos().setX(_x);
	//e->pos().setY(_y);
	cm.mouse_moved(_x, _y);

	//if Preference::isSet("guiLogMouseMove")
	//if ( m_need_motionlog )
		//dicmdCanvasMouseMove(e->pos()).log();
	/**/
	
    update();
}

void canvas::wheelEvent(QWheelEvent* e)
{
    //fixme need log?
    m_renderer->zoom((e->delta()/120),e->pos());
    update();
}

void canvas::mouseDoubleClickEvent(QMouseEvent* e)
{
    //if(!Application::is_replay_mode())
       //dicmdCanvasMouseDblClick(e->pos()).log();
   
    cm.mouse_dbl_clicked(e->pos().x(),e->pos().y());
    //if(!Application::is_log_mode())
    update();
}

void canvas::mouseReleaseEvent(QMouseEvent* e)
{
    cm.mouse_released(e->pos().x(),e->pos().y());
    //dicmdCanvasMouseDblClick(e->pos()).log();
    update();
}

void canvas::on_update()
{
    cm.update_tookplace();
    update();
}

void canvas::paintEvent(QPaintEvent*)
{
    m_renderer->render();
    //update(); why?
}

void canvas::invoke_create_line()
{
    cm.activate_command(cm.find_command("incmdCreateObjLine"));
}

void canvas::invoke_create_rect()
{
    cm.activate_command(cm.find_command("incmdCreateObjRectangle"));
}

void canvas::invoke_create_ellipse()
{
    cm.activate_command(cm.find_command("incmdCreateObjEllipse"));
}

void canvas::invoke_create_polygon()
{
    cm.activate_command(cm.find_command("incmdCreateObjPolygon"));
}

void canvas::invoke_select_by_region()
{
    cm.activate_command(cm.find_command("incmdSelectShapesByRegion"));
}

void canvas::invoke_select_by_point()
{
    cm.disactivate_active_command();
	//cm.activate_command(cm.find_command("incmdSelectUnderCursoer"));
}

void canvas::invoke_save()
{
    cm.activate_command(cm.find_command("incmdDesignSave"));
}

void canvas::invoke_load()
{
    cm.activate_command(cm.find_command("incmdDesignLoad"));
}

void canvas::reset()
{
    cm.activate_command(cm.find_command("incmdDesignNew"));
}

void canvas::invoke_delete()
{
	cm.activate_command(cm.find_command("incmdDeleteShape"));
}

void canvas::abordCommand()
{
    cm.activate_command(cm.find_command("dicmdAbortActiveCommand"));
}

void canvas::invoke_copy()
{
    cm.activate_command(cm.find_command("incmdObjRelocateByCopy"));
}

void canvas::invoke_move()
{
    cm.activate_command(cm.find_command("incmdObjRelocateByMove"));
}
