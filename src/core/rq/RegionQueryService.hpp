#ifndef REQGION_QUERY_SERVICE_HPP
#define REQGION_QUERY_SERVICE_HPP

#include "RQtree.hpp"

#include "../ishape.hpp"
#include "../service.hpp"

#ifdef NO_RQ
    #include "../iobject_pool.hpp"
#endif

class RegionQuery : public Service<RegionQuery>
{
public:
	void insertObject(IShape*);
	void removeObject(IShape*);
	IShape* getShapeUnderPos(const QPoint&) const;
	std::vector<IShape*> getShapesUnderRect(const QRect&) const;
	void clear();
	void shutDown() override;

#ifdef NO_RQ
    IObjectPoolPtr m_ws;
    void setWS(IObjectPoolPtr ws);
#endif

    RegionQuery();
private:
	rq::RQtreePtr<IShape> m_tree = nullptr;
};

#endif
