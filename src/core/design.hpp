#ifndef DESIGN_HPP
#define DESIGN_HPP

#include "iobject_pool.hpp"
#include "ishape.hpp"

#include <QObject>
#include <vector>

class Design final : public ObjectPoolBase
{
  public:
    //
    //	Interface
    //
    virtual ~Design() = default;

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

using DesignPtr = std::shared_ptr<Design>;

#endif
