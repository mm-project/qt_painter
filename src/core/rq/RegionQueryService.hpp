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

    RegionQuery();

  private:
    rq::RQtreePtr<IShape> m_tree = nullptr;
};
#endif //ifndef DUMMY_RQ

//Dummy RQ, implements same interface but in dummy way by iteratring over all objects in design
#ifdef DUMMY_RQ
#include "../iobject_pool.hpp"
class RegionQuery : public Service<RegionQuery>
{
  public:
    void insertObject(IShapePtr) {}
    void removeObject(IShapePtr) {}

    IShapePtr getShapeUnderPos(const QPoint &) const
    {
      for (auto shape : m_ws->getObjects())
          if (shape->contains(point))
              return shape;

      return nullptr;
    }

    std::vector<IShapePtr> getShapesUnderRect(const QRect &)
    {
      std::vector<IShapePtr> shapes;
      for (auto shape : m_ws->getObjects())
      {
          bool contains = true;

          for (auto point : shape->getPoints())
              if (!box.contains(point))
                  contains = false;

          if (contains)
              shapes.push_back(shape);
      }
      return shapes;
    }

    void clear() {  m_ws->getObjects().clear(); }
    void shutDown() override {}
    int getSize() const { m_ws->getObjects().size();}

    RegionQuery() {}
    void setWS(IObjectPool ws) {m_ws = ws;}

  private:
    IObjectPool m_ws;
};
#endif //ifdef DUMMY_RQ



#endif // REQGION_QUERY_SERVICE_HPP
