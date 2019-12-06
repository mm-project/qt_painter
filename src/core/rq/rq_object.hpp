#ifndef POINT_HPP
#define POINT_HPP

#include "../shapes.hpp"

#include <memory>
#include <iostream>

namespace rq {

/*
// change to point vector
template <typename T>
class CPoint
{
public:
	CPoint(const T& x = 0, const T& y = 0)
		: m_x(x),
		  m_y(y)
	{}

	inline T x() const
	{
		return m_x;
	}

	inline T y() const
	{
		return m_y;
	}

	inline void setX(const T& x)
	{
		m_x = x;
	}

	inline void setY(const T& y)
	{
		m_y = y;
	}

	T operator[](int i) const
	{
		if (i > 2)
			// throw;
			return -1;
		if (i == 0)
			return m_x;
		return m_y;
	}

	bool operator==(const CPoint<T>& point)
	{
		return (point.x() == m_x && point.y() == m_y);
	}

private:
	T m_x;
	T m_y;
};
*/

// because qt dont have operator<
class CPoint : public QPoint
{
public:
	CPoint() = default;
	explicit CPoint(QPoint p)
	{
		rx() = p.x();
		ry() = p.y();
	}

public:
	bool operator<(const CPoint& p) const
	{
		return (x() < p.x() && y() < p.y());
	}

	bool operator<(const QPoint& p) const
	{
		return (x() < p.x() && y() < p.y());
	}

	QPoint get() const
	{ 
		return QPoint(x(), y()); 
	}
};



//
//	Interface for IShape wrapper
//
class IRQobject
{
public:
	IRQobject() = default;

	virtual CPoint at(int) const = 0;

	virtual bool contains(const CPoint&) const = 0;

	virtual IShapePtr getObject() const = 0;

	virtual bool intersects(const QRect&) const = 0;
};

using RQobjectPtr = std::shared_ptr<IRQobject>;


//
//
//
class RQline : public IRQobject
{
public:

	RQline() = default;

	RQline(IShapePtr p) : m_object(p) {}

	virtual IShapePtr getObject() const override
	{ 
		return m_object;  
	}


	virtual CPoint at(int i) const override
	{
		if (i > 2)
			// throw;
			return CPoint();

		if (i == 0)
			return CPoint(static_cast<Line*>(m_object.get())->getP1());
		return CPoint(static_cast<Line*>(m_object.get())->getP2());
	}

	virtual bool contains(const CPoint& point) const override
	{
		QPoint p1 = static_cast<Line*>(m_object.get())->getP1();
		QPoint p2 = static_cast<Line*>(m_object.get())->getP2();
		QRectF bbox(p1, p2);
		if (!bbox.contains(point))
			// vertical and horizontal case
			return point.x() == p1.x() || point.x() == p2.x() || point.y() == p1.y() || point.y() == p2.y();
		float x = (float) (point.x() - p1.x()) / (p2.x() - p1.x());
		float y = (float) (point.y() - p1.y()) / (p2.y() - p1.y());
		float out =  x / y  * 100000 / 100000;
		std::cout << out << std::endl;
		bool b = out > 0.95 && out < 1.25;
		if (!b)
			// vertical and horizontal case
			return point.x() == p1.x() || point.x() == p2.x() || point.y() == p1.y() || point.y() == p2.y();
		return b;
	}


	virtual bool intersects(const QRect& oRect) const override
	{
		const QPoint p1 = static_cast<Line*>(m_object.get())->getP1();
		const QPoint p2 = static_cast<Line*>(m_object.get())->getP2();

		return intersectsLine(oRect.topLeft(), oRect.topRight())
			|| intersectsLine(oRect.topLeft(), oRect.bottomLeft())
			|| intersectsLine(oRect.bottomLeft(), oRect.bottomRight())
			|| intersectsLine(oRect.bottomRight(), oRect.topRight())
			|| oRect.contains(p1) || oRect.contains(p2);
	}
	
private:
	bool intersectsLine(QPoint a1, QPoint b1) const
	{
		const QPoint a0 = static_cast<Line*>(m_object.get())->getP1();
		const QPoint b0 = static_cast<Line*>(m_object.get())->getP2();

		float d = (b0.x() - a0.x()) * (b1.y() - a1.y()) - (b0.y() - a0.y()) * (b1.x() - b0.x());

		if (d == 0)
			return false;

		float q = (a0.y() - a1.y()) * (b1.x() - a1.x()) - (a0.x() - a1.x()) * (b1.y() - a1.y());
		float r = q / d;
		q = (a0.y() - a1.y()) * (b0.x() - a0.x()) - (a0.x() - a1.x()) * (b0.y() - a0.y());
		float s = q / d;
		if (r < 0 || r > 1 || s < 0 || s > 1)
			return false;

                return true;
    }

	IShapePtr m_object;
};

//
//
//
class RQrect : public IRQobject
{
public:
	RQrect() = default;
	RQrect(IShapePtr p) : m_object(p) {}

public:
	virtual CPoint at(int) const override;
	virtual bool contains(const CPoint&) const override;
	virtual IShapePtr getObject() const override;
	virtual bool intersects(const QRect& oRect) const override;

private:
	IShapePtr m_object;
};

//
//	
//
class RQellipse : public IRQobject
{
public:
	RQellipse() = default;
	RQellipse(IShapePtr p) : m_object(p) {}

public:
	virtual CPoint at(int) const override;
	virtual bool contains(const CPoint&) const override;
	virtual IShapePtr getObject() const override;
	virtual bool intersects(const QRect& oRect) const override;

private:
	IShapePtr m_object;
};

//
//	
//
class RQpolygon : public IRQobject
{
public:
	RQpolygon() = default;
	RQpolygon(IShapePtr p) : m_object(p) {}

public:
	virtual CPoint at(int) const override;
	virtual bool contains(const CPoint&) const override;
	virtual IShapePtr getObject() const override;
	virtual bool intersects(const QRect& oRect) const override;

private:
	IShapePtr m_object;
};

}
#endif
