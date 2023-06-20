#pragma once

#ifndef DUMMY_RQ
#include "kdtree.hpp"
#endif

#include "../ishape.hpp"
#include "../service.hpp"

#ifdef DUMMY_RQ
#include "../iobject_pool.hpp"
#endif

class RegionQuery : public Service<RegionQuery>
{
  public:
    void insertObject(IShapePtr);
    void removeObject(IShapePtr);
    std::vector<IShapePtr> getShapeUnderPos(const QPoint &) const;
    std::vector<IShapePtr> getShapesUnderRect(const QRect &) const;
    void clear();
    void shutDown() override;
    int getSize() const;

#ifdef DUMMY_RQ
    ObjectPoolPtr m_ws;
#endif

    RegionQuery();

#ifndef DUMMY_RQ
  private:
    KDtreePtr<IShapePtr> m_tree = nullptr;
#endif

};


