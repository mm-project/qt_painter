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
class line : public IShape
{
public:
	inline line(QLine = QLine(), ShapeProperties = ShapeProperties());
	virtual ~line() = default;

public:
	virtual line* clone() override;
	virtual void draw(QPainter*) override;

public:
	virtual void reset() override;
	virtual void addPoint(const QPoint&) override;

private:
	void setP1(const QPoint&);
	void setP2(const QPoint&);

public: 
	QPoint getP1() const { return m_object.p1(); }
	QPoint getP2() const { return m_object.p2(); }
	
        virtual void moveCenterToPoint(QPoint& p ) {
                //m_object.moveCenter(p);
        }

	virtual ObjectType getType() const override { return LINE; }
	
	//FIXME need proper fix and member handling
	virtual std::vector<QPoint> getPoints() { return std::vector<QPoint>(2) =  {m_object.p1(),m_object.p2()}; }

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
	inline rectangle(QRect = QRect(), ShapeProperties = ShapeProperties());
	virtual ~rectangle() = default;

public:
	virtual rectangle* clone() override;
	virtual void draw(QPainter*) override;

public:
	virtual void reset() override;
	virtual void addPoint(const QPoint&) override;

public:
	void setTopLeft(const QPoint&);
	void setBottomRight(const QPoint&);

	QPoint getTopLeft() const;
	QPoint getBottomRight() const;

	bool contains(const QPoint& point) const { return m_object.contains(point); }
	bool intersects(const QRect& oRect) const { return m_object.intersects(oRect); }
	virtual ObjectType getType() const override { return RECTANGLE; }

	virtual void moveCenterToPoint(QPoint& p ) {
                m_object.moveCenter(p);
        }
	
    //FIXME need proper fix and member handling
	virtual std::vector<QPoint> getPoints() { return std::vector<QPoint>(2) =  {getBottomRight(),getTopLeft()}; }

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
	inline ellipse(QRect = QRect(), ShapeProperties = ShapeProperties());
	virtual ~ellipse() = default;
  
public:
	virtual ellipse* clone() override;
	virtual void draw(QPainter*) override;

public:
	virtual void reset() override;
	virtual void addPoint(const QPoint&) override;

public:
	void setTopLeft(const QPoint&);
	void setBottomRight(const QPoint&);

	QPoint getTopLeft() const;
	QPoint getBottomRight() const;

	bool contains(const QPoint& point) const { return m_object.contains(point); }
	bool intersects(const QRect& oRect) const { return m_object.intersects(oRect); }
	virtual ObjectType getType() const override { return ELLIPSE; }
        
        virtual void moveCenterToPoint(QPoint& p ) {
                m_object.moveCenter(p);
        }

	    //FIXME need proper fix and member handling
	virtual std::vector<QPoint> getPoints() { return std::vector<QPoint>(2) =  {getBottomRight(),getTopLeft()}; }

private:
	QRect m_object;
	bool m_waitForSecondClick;
};

class polygon : public IShape 
{
public:

	inline polygon(QPolygon = QPolygon(), ShapeProperties = ShapeProperties());
	virtual ~polygon() = default;

public:
	virtual polygon* clone() override;
	virtual void draw(QPainter*) override;

public:
	virtual void reset() override;
	virtual void addPoint(const QPoint&) override;
	virtual void movePoint(const QPoint&) override;

	virtual ObjectType getType() const override { return POLYGON; }

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
               // m_object.moveCenter(p);
        }

    //FIXME need proper fix and member handling
	virtual std::vector<QPoint> getPoints() { return m_object.toStdVector(); }
	
private:
	QPolygon m_object;
	bool m_first = true;

};

#endif
