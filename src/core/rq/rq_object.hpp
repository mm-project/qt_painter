#ifndef POINT_HPP
#define POINT_HPP

#include "..\shapes.hpp"

#include <memory>

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

	virtual IShape* getObject() const = 0;
};

using RQobjectPtr = std::shared_ptr<IRQobject>;


//
//
//
class RQline : public IRQobject
{
public:

	RQline() = default;

	RQline(IShape* p) : m_object(static_cast<line*>(p)) {}

	virtual IShape* getObject() const override
	{ 
		return m_object;  
	}


	virtual CPoint at(int i) const override
	{
		if (i > 2)
			// throw;
			return CPoint();

		if (i == 0)
			return CPoint(m_object->getP1());
		return CPoint(m_object->getP2());
	}

	virtual bool contains(const CPoint& point) const override
	{
		QPoint p1 = m_object->getP1();
		QPoint p2 = m_object->getP2();

		float x = (float) (point.x() - p1.x()) / (p2.x() - p1.x());
		float y = (float) (point.y() - p1.y()) / (p2.y() - p1.y());
		return x == y;
	}

private:
	line* m_object;
};

//
//
//
class RQrect : public IRQobject
{
public:
	RQrect() = default;
	RQrect(IShape* p) : m_object(static_cast<rectangle*>(p)) {}

public:
	virtual CPoint at(int) const override;
	virtual bool contains(const CPoint&) const override;
	virtual IShape* getObject() const override;

private:
	rectangle* m_object;
};

//
//	
//
class RQellipse : public IRQobject
{
public:
	RQellipse() = default;
	RQellipse(IShape* p) : m_object(static_cast<ellipse*>(p)) {}

public:
	virtual CPoint at(int) const override;
	virtual bool contains(const CPoint&) const override;
	virtual IShape* getObject() const override;

private:
	ellipse* m_object;
};

//
//	
//
class RQpolygon : public IRQobject
{
public:
	RQpolygon() = default;
	RQpolygon(IShape* p) : m_object(static_cast<polygon*>(p)) {}

public:
	virtual CPoint at(int) const override;
	virtual bool contains(const CPoint&) const override;
	virtual IShape* getObject() const override;

private:
	polygon* m_object;
};

}
#endif