#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "basic_shape.hpp"
#include "command_manager.hpp"
#include "iobject_pool.hpp"

#include <QWidget>
#include <QPainter>

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
        virtual void keyPressEvent(QKeyEvent*) override;
        virtual void mousePressEvent(QMouseEvent*) override;
        virtual void mouseMoveEvent(QMouseEvent*) override;
        virtual void mouseDoubleClickEvent(QMouseEvent*) override;
        virtual void wheelEvent(QWheelEvent* event) override;

public slots:
        void current_type_changed();
        void invoke_create_line();
        void invoke_create_rect();
        void invoke_create_ellipse();
        void invoke_create_polygon();
        void invoke_select_by_region();
        
        void reset();
        void on_update();

private:
        bool is_runtime_mode;
        IObjectPoolPtr m_working_set;
        ObjectPoolSandboxPtr m_sandbox;
        command_manager* cm;
        renderer* m_renderer;
};




#endif
