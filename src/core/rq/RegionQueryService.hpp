#ifndef REQGION_QUERY_SERVICE_HPP
#define REQGION_QUERY_SERVICE_HPP

#include "RQtree.hpp"
#include "../ishape.hpp"
#include "../postman.hpp"

class WSCallbackData : public LeCallbackData
{
    public:
        IShape* get_data() { return m_data; }
        void set_data(IShape* s) { m_data = s; }
    private:
        IShape* m_data;
};

class RegionQuery
{
public:
	static RegionQuery& getInstance();

public:
	IShape* getShapeUnderPos(const QPoint&) const;
	std::vector<IShape*> getShapesUnderRect(const QRect&) const;
	void clear();

private:
	RegionQuery();
	RegionQuery(const RegionQuery&) = default;
	RegionQuery& operator=(const RegionQuery&) = default;
	void insertObject(LeCallbackData&);

private:
	static std::unique_ptr<RegionQuery> m_instance;
	rq::RQtreePtr<IShape> m_tree = nullptr;
	//rq::RQtreePtr<line> m_tree = nullptr;
};

#endif
