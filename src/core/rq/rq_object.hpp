#ifndef POINT_HPP
#define POINT_HPP

#include "shapes.hpp"

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

// becouse qt dont have operator<
class CPoint : public QPoint
{
public:
	CPoint() = default;
	explicit CPoint(QPoint p) : m_point(p) {}

public:
	bool operator<(const CPoint& p) const
	{
		return (m_point.x() < p.x() && m_point.y() < p.y());
	}

	bool operator<(const QPoint& p) const
	{
		return (m_point.x() < p.x() && m_point.y() < p.y());
	}
private:
	QPoint m_point;
};


class IRQobject
{
public:
	IRQobject() = default;

	virtual CPoint at(int) const = 0;

	virtual bool contains(const CPoint&) const = 0;

	virtual IShape* getObject() const = 0;
};

template <typename T>
using IRQobjectPtr = std::shared_ptr<IRQobject<T>>;

class RQline : public IRQobject
{
public:

	RQline() = default;

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
		return true;

		QPoint p1 = m_object->getP1();
		QPoint p2 = m_object->getP2();

		return (((point.x() - p1.x()) / (p2.x() - p1.x())) 
			== ((point.y() - p1.y()) / (p2.y() - p1.y())));
	}

private:
	line* m_object;
};
}
#endif