#pragma once

#include <memory>
#include <functional>
#include <unordered_map>
#include "spd_core.hpp"
#include "spd_runtime.hpp"
#include "tlv.hpp"

// Type definitions
class DesignObject;
using CreateFn = std::function<std::shared_ptr<DesignObject>()>;

// ---- Object ID / Handle -------------------------------------------------
using Handle = std::uint64_t;

// IDesignObject interface
class DesignObject
{
public:
    virtual ~DesignObject() = default;
    Handle id{};
    Handle owner{};

    virtual const ClassGuid& guid() const = 0;
    virtual std::uint32_t objectVersion() const { return 1; }

    virtual void writeTlv(TlvWriter&) const {}
    virtual void readTlv(TlvReader&) {}
};

// Object creator registry
class Registry 
{
public:
    static Registry& instance() {
        static Registry reg;
        return reg;
    }
    
    void registerType(const ClassGuid& g, CreateFn fn) {
        types_[g] = fn;
    }
    std::shared_ptr<DesignObject> create(const ClassGuid& g) const {
        auto it = types_.find(g); 
        if (it == types_.end()) 
            return nullptr; 
        return (it->second)();
    }
private:
    std::unordered_map<ClassGuid, CreateFn, GuidHash, GuidEq> types_;
};

// Helper macro to declare a static GUID and auto-register a class
#define SPD_DECLARE_TYPE(GUID_INIT) \
public: \
    static const ClassGuid& staticGuid() { static const ClassGuid g = GUID_INIT; return g; } \
    const ClassGuid& guid() const override { return staticGuid(); } 

#define SPD_REGISTER_TYPE(T) \
namespace { \
struct T##_registrar { \
    T##_registrar() { Registry::instance().registerType(T::staticGuid(), \
    [](){ return std::make_unique<T>(); }); } \
}; \
static T##_registrar g_##T##_registrar; \
}

using DesignObjectPtr = std::shared_ptr<DesignObject>;