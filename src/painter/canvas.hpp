#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "basic_shape.hpp"
#include "command.hpp"
#include "command_manager.hpp"

#include <QWidget>
#include <QPainter>

class rectangle;
class line;
class working_set;
class runtime_environment;

// REPONSIBLE FOR VIEWPORT CONTROLL
class renderer
{
    //Q_OBJECT
    public:
        //WPainter(QPaintDevice* p):QPainter(p){
        renderer (QWidget* w) { 	
            m_zoom_factor = 1;
            painter = new QPainter(w);
            parent_windget = w;
        }
        
        ~renderer() {
                
        }
        
        //void drawRect(const QRect& r) {
        //	QRect r2(r);
        //	r2.setHeight(r.height());
        //	r2.setWidth(r.width());
        //	QPainter::drawRect(r2);
        //}
        
        void start() {
            painter->begin(parent_windget);
        }
        
        void stop() {
            painter->end();
        }
        
        void incr_zoom_factor() {
            m_zoom_factor++;
        }
        
        void decr_zoom_factor() {
            m_zoom_factor--;
        }
        
        //void pan(int x, int y) {
        //	
        //}
        
        QPainter*  get_painter() {
            return painter;
        }
        
    private:
            int m_zoom_factor;
            
    private:
            QPainter* painter;
            QWidget* parent_windget;
};

// ACTUALL CANVAS
class canvas : public QWidget
{
        Q_OBJECT

public:
        canvas(QWidget* = 0);

public:
        virtual void paintEvent(QPaintEvent*) override;
        virtual void mousePressEvent(QMouseEvent*) override;
        virtual void mouseMoveEvent(QMouseEvent*) override;
        virtual void mouseDoubleClickEvent(QMouseEvent*) override;
        virtual void wheelEvent(QWheelEvent* event) override;

public slots:
        void current_type_changed();
        void create_line();
        void create_rect();
        void create_ellipse();
        void create_polygon();
        void reset();

private:
        bool is_runtime_mode;

        working_set* m_working_set;
        runtime_environment* m_runtime_environment;
		
        command_manager* cm;
        renderer* m_renderer;
};




#endif
