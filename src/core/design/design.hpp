#pragma once

#include <memory>
#include <unordered_map>

#include "../iobject_pool.hpp"
#include "design_object.hpp"
#include "spd_runtime.hpp"

class Design;
using DesignPtr = std::shared_ptr<Design>;
using DesignWPtr = std::weak_ptr<Design>;
using ObjectID = Handle;

// ---- Database: hold objects, serialize to OBJT, build simple index ----------
struct IndexEntry 
{ 
    Handle handle; 
    std::uint64_t offset; 
    std::uint32_t length; 
};

class Design final : public IObjectPool
{
public:
    // IObjectPool interface
    virtual std::string getName() const noexcept override;

    IShapePtr addObject(IShapePtr s) override;
    virtual void removeObject(IShapePtr) noexcept override;
    virtual void clear() noexcept override;
    virtual std::vector<IShapePtr> getObjects() const noexcept override;

    virtual void dumpToFile(const std::string&) const override;

    // Design interface
    ObjectID addObject(DesignObjectPtr s);
    void removeObject(DesignObjectPtr) noexcept;
    void removeObject(ObjectID) noexcept;
    std::vector<ObjectID> getObjectIDs() const noexcept;
    DesignObjectPtr getObject(ObjectID) const noexcept;

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

private:
    // Serialization methods
    void write(spd::ContainerWriter& cw) const;
    void read(spd::ContainerReader& cr);

    template<class T, class...Args>
    T* emplace(Handle h, Args&&...args);

private:
    // TODO: do we need to make this shared_ptr ?
    std::unordered_map<Handle, std::shared_ptr<DesignObject>> objects;

};