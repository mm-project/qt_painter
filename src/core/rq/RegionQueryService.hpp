#ifndef REQGION_QUERY_SERVICE_HPP
#define REQGION_QUERY_SERVICE_HPP

#include "RQtree.hpp"
#include "../basic_shape.hpp"

class RegionQuery
{
public:
	static RegionQuery& getInstance();

public:
	void insertObject(IShape*);
	IShape* getShapeUnderPos(const QPoint&) const;
	std::vector<IShape*> getShapesUnderRect(const QRect&) const;

private:
	RegionQuery();
	RegionQuery(const RegionQuery&) = default;
	RegionQuery& operator=(const RegionQuery&) = default;

private:
	static std::unique_ptr<RegionQuery> m_instance;
	rq::RQtreePtr<IShape> m_tree = nullptr;
	//rq::RQtreePtr<line> m_tree = nullptr;
};

#endif