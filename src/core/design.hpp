#ifndef DESIGN_HPP
#define DESIGN_HPP

#include "iobject_pool.hpp"
#include "ishape.hpp"

#include <QObject>
#include <vector>

class Design1 final : public ObjectPoolBase
{
  public:
    //
    //	Interface
    //
    virtual ~Design1() = default;

  public:
    IShapePtr addObject(IShapePtr) override;
    std::string getName() const noexcept override;

  public:
    bool isSaved() const noexcept
    {
        return m_is_saved;
    }
    void save()
    {
        m_is_saved = true;
    }

  private:
    bool m_is_saved = false;
};

using Design1Ptr = std::shared_ptr<Design1>;

#endif
