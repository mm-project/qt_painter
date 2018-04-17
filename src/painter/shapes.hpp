#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "basic_shape.hpp"

#include <QObject>
#include <QLine>
#include <QRect>
#include <QMouseEvent>
#include <QPolygonF>

class line : public QObject,
        public basic_shape
{
        Q_OBJECT

public:
        line(QLine, basic_properties);

        virtual void mousePressEvent(QMouseEvent*);
public:
        virtual line* clone();

        virtual void draw(QPainter*);
public:
        virtual void reset();
        virtual void set_pos1(const QPoint&);
        virtual void set_pos2(const QPoint&);

private:
        QLine m_object;
};

class rectangle : public basic_shape
{
public:
        rectangle(QRect, basic_properties);

        virtual ~rectangle();

public:
        virtual rectangle* clone();

        virtual void draw(QPainter*);
public:
        virtual void reset();
        virtual void set_pos1(const QPoint&);
        virtual void set_pos2(const QPoint&);
private:
        QRect m_object;
};

class ellipse : public basic_shape
{
public:
        ellipse(QRect, basic_properties);

        virtual ~ellipse() {}

public:
        virtual ellipse* clone();

        virtual void draw(QPainter*);
public:
        virtual void reset();
        virtual void set_pos1(const QPoint&);
        virtual void set_pos2(const QPoint&);
private:
        QRect m_object;
};

class polygon : public basic_shape
{
public:
        polygon(QPolygonF, basic_properties);

public:
        virtual polygon* clone();

        virtual void draw(QPainter*);
public:
        virtual void reset();
        virtual void set_pos1(const QPoint&);
        virtual void set_pos2(const QPoint&);
private:
        QPolygonF m_object;
};
#endif
