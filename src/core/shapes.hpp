#ifndef SHAPES_HPP
#define SHAPES_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
#include "ishape.hpp"

// Qt
#include <QObject>
#include <QLine>
#include <QPoint>
#include <QRect>
#include <QMouseEvent>
#include <QPolygonF>


//stl
#include <vector>


///////////////////////////////////////////////////////////////////////////////
//
// @class line, wrapper for OA/Qt object 
//
class Line : public IShape
{
public:
	inline Line(QLine = QLine(), ShapeProperties = ShapeProperties());
	virtual ~Line() = default;

public:
	Line* clone() override;
	void draw(QPainter*) override;

public:
	void reset() override;
	void addPoint(const QPoint&) override;

private:
	void setP1(const QPoint&);
	void setP2(const QPoint&);

public: 
	QPoint getP1() const; 
	QPoint getP2() const;

	virtual ObjectType getType() const override 
	{
		return LINE; 
	}
	
	virtual void moveCenterToPoint(QPoint& p ) {
			//m_object.moveTo(p);
	}

    #ifdef NO_RQ
    virtual bool contains(const QPoint& point) const {return false;}    
    #endif

	//FIXME need proper fix and member handling
	std::vector<QPoint> getPoints() override
	{
		return std::vector<QPoint>(2) =  {m_object.p1(),m_object.p2()}; 
	}

private:
	QLine m_object;
	bool m_waitForSecondClick;
};

///////////////////////////////////////////////////////////////////////////////
//
// @class rectangle, wrapper for OA/Qt object 
//
class Rectangle : public IShape
{
public:
	inline Rectangle(QRect = QRect(), ShapeProperties = ShapeProperties());
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

///////////////////////////////////////////////////////////////////////////////
//
// @class ellipse, wrapper for OA/Qt object 
//
class Ellipse : public IShape
{
public:
	inline Ellipse(QRect = QRect(), ShapeProperties = ShapeProperties());
	virtual ~Ellipse() = default;
  
public:
	Ellipse* clone() override;
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

	virtual ObjectType getType() const override 
	{ 
		return ELLIPSE; 
	}

	virtual void moveCenterToPoint(QPoint& p ) {
			m_object.moveTo(p);
	}

	std::vector<QPoint> getPoints() override 
	{ 
		return std::vector<QPoint>(2) =  {getBottomRight(),getTopLeft()}; 
	}

private:
	QRect m_object;
	bool m_waitForSecondClick;
};

class Polygon : public IShape 
{
public:

	inline Polygon(QPolygon = QPolygon(), ShapeProperties = ShapeProperties());
	virtual ~Polygon() = default;

public:
	Polygon* clone() override;
	void draw(QPainter*) override;

public:
	void reset() override;
	void addPoint(const QPoint&) override;
	void movePoint(const QPoint&) override;

	ObjectType getType() const override 
	{ 
		return POLYGON; 
	}

	QPoint getTopLeft() const;
	QPoint getBottomRight() const;

	bool contains(const QPoint& point) const
	{
		return m_object.boundingRect().contains(point); 
	}

	bool intersects(const QRect& oRect) const 
	{
		return m_object.boundingRect().intersects(oRect); 
	}

	virtual void moveCenterToPoint(QPoint& p ) {
		   // m_object.moveTo(p);
	}

    //FIXME need proper fix and member handling
	std::vector<QPoint> getPoints() override 
	{ 
		return m_object.toStdVector(); 
	}
	
private:
	QPolygon m_object;
	bool m_first = true;

};

#endif
