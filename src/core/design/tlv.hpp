#pragma once

#include "spd_core.hpp"

#include <array>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <optional>
#include <iostream>

// ============================ TLV & Registry ================================
using ClassGuid = std::array<std::uint8_t,16>;

struct GuidHash 
{ 
    std::size_t operator()(const ClassGuid& g) const noexcept 
    { 
        std::uint64_t h=1469598103934665603ull; 
        for (auto b:g)
        { 
            h^=b; 
            h*=1099511628211ull;
        } 
        return (size_t)h; 
    } 
};

struct GuidEq 
{ 
    bool operator()(const ClassGuid& a,const ClassGuid& b) const noexcept 
    { 
        for (size_t i=0;i<16;++i) 
        { 
            if (a[i]!=b[i]) 
                return false; 
        } 
        return true; 
    } 
};

constexpr ClassGuid make_guid(std::initializer_list<std::uint8_t> bytes)
{ 
    ClassGuid g{}; 
    size_t i=0; 
    for(auto b:bytes){ if(i<16) g[i++]=b; } 
    return g; 
}

// VarUInt32 (LEB128)
void write_varuint32(spd::OStream& os, std::uint32_t v);
std::uint32_t read_varuint32(spd::IStream& is);

using FieldId = std::uint16_t; 

class TlvWriter 
{ 
public: 
    explicit TlvWriter(spd::OStream& os);
    void field(FieldId id, const void* data, std::size_t len);
    void u8(FieldId id, std::uint8_t v);
    void u16(FieldId id, std::uint16_t v);
    void u32(FieldId id, std::uint32_t v);
    void u64(FieldId id, std::uint64_t v);
    void f32(FieldId id, float v);
    void f64(FieldId id, double v);
    void bytes(FieldId id, const std::vector<std::uint8_t>& v);
    void str(FieldId id, const std::string& s);
    template<class T> 
    void pod(FieldId id, const T& v) 
    { 
        static_assert(std::is_trivially_copyable<T>::value, "pod only"); 
        field(id, &v, sizeof(T)); 
    }
private: 
    spd::OStream& os_; 
};

class TlvReader 
{ 
public: 
    struct Item 
    { 
        FieldId id; 
        std::uint32_t len; 
        std::uint64_t valueOffset; 
    };
    
    TlvReader(spd::IStream& is, std::uint64_t start, std::uint64_t end);
    bool next(Item& out);
    template<class T> T read_primitive(const Item& it) 
    { 
        if (sizeof(T) != it.len) 
            throw std::runtime_error("TLV size"); 
        T v{}; 
        read_raw(it, &v, sizeof(T)); 
        return v; 
    }
    template<class T> void read_pod(const Item& it, T& v) 
    { 
        if (sizeof(T) != it.len) 
            throw std::runtime_error("TLV size"); 
        read_raw(it, &v, sizeof(T)); 
    }
    std::string read_string(const Item& it);
    std::vector<std::uint8_t> read_bytes(const Item& it);
    void read_raw(const Item& it, void* dst, std::size_t n);
private: 
    spd::IStream& is_; 
    std::uint64_t cur_, end_; 
};
