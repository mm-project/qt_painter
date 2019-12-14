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
	void insertObject(IShapePtr);
	void removeObject(IShapePtr);
	IShapePtr getShapeUnderPos(const QPoint&) const;
	std::vector<IShapePtr> getShapesUnderRect(const QRect&) const;
	void clear();
	void shutDown() override;
	int getSize() const;

#ifdef NO_RQ
    IObjectPoolPtr m_ws;
    void setWS(IObjectPoolPtr ws);
#endif

    RegionQuery();
private:
	rq::RQtreePtr<IShape> m_tree = nullptr;
};

#endif
