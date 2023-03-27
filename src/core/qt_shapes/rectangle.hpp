#pragma once

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "../ishape.hpp"

// Qt
#include <QObject>
#include <QPoint>
#include <QRect>
#include <QMouseEvent>

///////////////////////////////////////////////////////////////////////////////
//
// @class rectangle, wrapper for OA/Qt object 
//
class Rectangle : public IShape
{
public:
	Rectangle(QRect = QRect(), ShapeProperties = ShapeProperties());
	virtual ~Rectangle() = default;

public:
	Rectangle* clone() override;
	void draw(QPainter*) override;

public:
	void reset() override;
	void addPoint(const QPoint&) override;

public:
	void setTopLeft(const QPoint&);
	void setBottomRight(const QPoint&);

	QPoint getTopLeft() const;
	QPoint getBottomRight() const;

	bool contains(const QPoint& point) const { return m_object.contains(point); }
	bool intersects(const QRect& oRect) const { return m_object.intersects(oRect); }

	ObjectType getType() const override 
	{ 
		return RECTANGLE; 
	}

	virtual void moveCenterToPoint(QPoint& p ) {
                m_object.moveTo(p);
        }
	
    //FIXME need proper fix and member handling
	std::vector<QPoint> getPoints() override
	{
		return std::vector<QPoint>(2) =  {getBottomRight(),getTopLeft()}; 
	}

private:
	QRect m_object;
	bool m_waitForSecondClick;
};