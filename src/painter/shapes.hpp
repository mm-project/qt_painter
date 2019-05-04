#ifndef SHAPES_HPP
#define SHAPES_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "basic_shape.hpp"

// Qt
#include <QObject>
#include <QLine>
#include <QRect>
#include <QMouseEvent>
#include <QPolygonF>

///////////////////////////////////////////////////////////////////////////////
//
// @class line, wrapper for OA/Qt object 
//
class line : public IShape
{
public:
	line(QLine = QLine(), ShapeProperties = ShapeProperties());
	virtual ~line();

public:
	virtual line* clone() override;
	virtual void draw(QPainter*) override;

public:
	virtual void reset() override;
	virtual void addPoint(const QPoint&) override;

public:
	void setP1(const QPoint&);
	void setP2(const QPoint&);

	QPoint getP1() const { return m_object.p1(); }
	QPoint getP2() const { return m_object.p2(); }

private:
	QLine m_object;
	bool m_waitForSecondClick;
};

///////////////////////////////////////////////////////////////////////////////
//
// @class rectangle, wrapper for OA/Qt object 
//
class rectangle : public IShape
{
public:
	rectangle(QRect = QRect(), ShapeProperties = ShapeProperties());
	virtual ~rectangle();

public:
	virtual rectangle* clone() override;
	virtual void draw(QPainter*) override;

public:
	virtual void reset() override;
	virtual void addPoint(const QPoint&) override;

public:
	void setTopLeft(const QPoint&);
	void setBottomRight(const QPoint&);

private:
	QRect m_object;
	bool m_waitForSecondClick;
};

///////////////////////////////////////////////////////////////////////////////
//
// @class ellipse, wrapper for OA/Qt object 
//
class ellipse : public IShape
{
public:
	ellipse(QRect = QRect(), ShapeProperties = ShapeProperties());
	virtual ~ellipse();

public:
	virtual ellipse* clone() override;
	virtual void draw(QPainter*) override;

public:
	virtual void reset() override;
	virtual void addPoint(const QPoint&) override;

public:
	void setTopLeft(const QPoint&);
	void setBottomRight(const QPoint&);

private:
	QRect m_object;
	bool m_waitForSecondClick;
};

class polygon : public IShape 
{
public:
	polygon(QPolygonF = QPolygonF(), ShapeProperties = ShapeProperties());
	virtual ~polygon();

public:
	virtual polygon* clone() override;
	virtual void draw(QPainter*) override;

public:
	virtual void reset() override;
	virtual void addPoint(const QPoint&) override;

private:
	QPolygonF m_object;
};

#endif
