#include "canvas.hpp"

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
        : QWidget(p), is_runtime_mode(false), m_working_set(0)
{
        setMouseTracking(true);
        m_working_set = new working_set;
        m_runtime_environment = new runtime_environment();
}

void canvas::mousePressEvent(QMouseEvent* e)
{
        if (is_runtime_mode) {
                m_runtime_environment->set_pos2(e->pos());
                m_working_set->add_object(m_runtime_environment);
                m_runtime_environment->reset();
                is_runtime_mode = false;
                update();
        } else {
                controller* c = controller::get_instance();
                m_runtime_environment->change_basic_properties(c->get_basic_properties());
                m_runtime_environment->set_pos1(e->pos());
                is_runtime_mode = true;
        }
}

void canvas::current_type_changed()
{
        controller* c = controller::get_instance();
        m_runtime_environment->change_object_type(c->get_object_type());
}

void canvas::mouseMoveEvent(QMouseEvent* e)
{
        if (is_runtime_mode) {
                m_runtime_environment->set_pos2(e->pos());
                update();
        }
}

void canvas::paintEvent(QPaintEvent*)
{
        QPainter painter(this);
        QRect rect(QPoint(0, 0), size());
        QBrush b(Qt::black);
        painter.setBrush(b);
        painter.drawRect(rect);

        std::vector<basic_shape*> shapes = m_working_set->get_objects();
        for (auto i = shapes.begin(); i != shapes.end(); ++i) {
                (*i)->draw(&painter);
        }
        m_runtime_environment->draw_runtime(&painter);
}

void canvas::create_line()
{
        m_runtime_environment->change_object_type(LINE);
}

void canvas::create_rect()
{
        m_runtime_environment->change_object_type(RECT);
}

void canvas::create_ellipse()
{
        m_runtime_environment->change_object_type(ELLIPSE);
}

void canvas::create_polygon()
{
        m_runtime_environment->change_object_type(POLYGON);
}

void canvas::reset()
{
        m_working_set->clear();
        update();
}

void canvas::mouseDoubleClicked(QMouseEvent*)
{
        //is_runtime_mode = false;
}

