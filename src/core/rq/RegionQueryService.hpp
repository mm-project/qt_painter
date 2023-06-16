#ifndef REQGION_QUERY_SERVICE_HPP
#define REQGION_QUERY_SERVICE_HPP

#include "RQtree.hpp"

#include "../ishape.hpp"
#include "../service.hpp"


#ifndef DUMMY_RQ
class RegionQuery : public Service<RegionQuery>
{
  public:
    void insertObject(IShapePtr);
    void removeObject(IShapePtr);
    IShapePtr getShapeUnderPos(const QPoint &) const;
    std::vector<IShapePtr> getShapesUnderRect(const QRect &) const;
    void clear();
    void shutDown() override;
    int getSize() const;

  private:
    RegionQuery();

  private:
    rq::RQtreePtr<IShape> m_tree = nullptr;
};
#endif


#ifdef DUMMY_RQ
#include "../iobject_pool.hpp"
class RegionQuery : public Service<RegionQuery>
{
  public:
    void insertObject(IShapePtr) {}
    void removeObject(IShapePtr) {}

    IShapePtr getShapeUnderPos(const QPoint & point) const
    {
        for (auto shape : m_ws->getObjects())
            if (shape->contains(point))
                return shape;

        return nullptr;
    }

    std::vector<IShapePtr> getShapesUnderRect(const QRect & box) const
    {
      std::vector<IShapePtr> shapes;
      // Iterte over all objects in design and check if current shapes point are inside query box
      // consider it is, if at least 1 point is in that rect
      for (auto shape : m_ws->getObjects())
      {
          bool contains = false;

          for (auto point : shape->getPoints())
              if (box.contains(point))
                  contains = true;
                  //break here actually

          if (contains)
              shapes.push_back(shape);
      }
      return shapes;
    }


    void clear() { m_ws->clear(); }
    void shutDown() {}
    int getSize() const { return m_ws->getObjects().size(); }

    void setWS(ObjectPoolPtr ws) {m_ws = ws;}

    RegionQuery() {};

  private:
    ObjectPoolPtr m_ws;

};
#endif //ifdef DUMMY_RQ


#endif
