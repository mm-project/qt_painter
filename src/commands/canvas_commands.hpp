#ifndef CANVAS_COMMANDS_HPP
#define CANVAS_COMMANDS_HPP

#include "command_manager.hpp"
#include "direct_command_base.hpp"

#include "../core/renderer.hpp"


/*
dicmdCanvasOriginPanLeft
dicmdCanvasOriginPanRight
dicmdCanvasOriginPanUp
dicmdCanvasOriginPanDown
*/

template <panDirection T>
class dicmdCanvasOrigin: public NonTransactionalDirectCommandBase
{
    public:        
        virtual std::string get_name() {
            return "dicmdCanvasOrigin"+panDirection2str(T);
        }

        virtual void execute() {
             command_manager::get_instance()->get_main_renderer()->pan(T);
             command_manager::get_instance()->get_main_widget()->update();
        }

};

/*
dicmdCanvasZoomIn -to_point
dicmdCanvasZoomOut -from_point
*/

template <zoomDirection T>
class dicmdCanvasViewport: public NonTransactionalDirectCommandBase
{
    QPoint m_p;
    public:

        dicmdCanvasViewport<T>() {
            add_option("-point",new PointCommandOptionValue(QPoint(0,0)));
        }
        
        dicmdCanvasViewport<T>(const QPoint& p):NonTransactionalDirectCommandBase("-point",new PointCommandOptionValue(p)) 
        { m_p = p; }
        

        virtual std::string get_name() {
            return "dicmdCanvasViewport"+zoomDirection2str(T);
        }

        virtual void execute() {
            m_p = GET_CMD_ARG(PointCommandOptionValue,"-point");
            command_manager::get_instance()->get_main_renderer()->zoom_internal(T,m_p);
            command_manager::get_instance()->get_main_widget()->update();
        }

};



#endif
