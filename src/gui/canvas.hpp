#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "../core/renderer.hpp"
#include "../core/iobject_pool.hpp"
#include "../core/ishape.hpp"
#include "../commands/command_manager.hpp"

#include <QWidget>
#include <QPainter>

// REPONSIBLE FOR VIEWPORT CONTROLL


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
        void invoke_select_by_point();
        void invoke_save();
        void invoke_load();
        
        void reset();
        void on_update();

private:
        bool is_runtime_mode;
        bool m_need_motionlog = false;
        IObjectPoolPtr m_working_set;
        ObjectPoolSandboxPtr m_sandbox;
        command_manager* cm;
        renderer* m_renderer;
        int m_scale = 15;
};


#endif
