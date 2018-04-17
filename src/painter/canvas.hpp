#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "basic_shape.hpp"

#include <QWidget>

class rectangle;
class line;
class working_set;
class runtime_environment;

class canvas : public QWidget
{
        Q_OBJECT

public:
        canvas(QWidget* = 0);

public:
        virtual void paintEvent(QPaintEvent*);
        virtual void mousePressEvent(QMouseEvent*);
        virtual void mouseMoveEvent(QMouseEvent*);

        virtual void mouseDoubleClicked(QMouseEvent*);

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
};

#endif
