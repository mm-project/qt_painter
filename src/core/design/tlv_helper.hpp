#pragma once

#include "tlv.hpp"
#include "spd_runtime.hpp"

// ---- Shapes: math & properties ---------------------------------------------
struct Vec2 
{ 
    double x{}, y{}; 
};

struct Vec3 
{ 
    double x{}, y{}, z{};
};

struct Rgba 
{ 
    std::uint8_t r{0}, g{0}, b{0}, a{255}; 
};

struct ShapeProps 
{ 
};

inline std::ostream& operator<<(std::ostream& os, const Rgba& c)
{ 
    return os<<"rgba("<<(int)c.r<<","<<(int)c.g<<","<<(int)c.b<<","<<(int)c.a<<")"; 
}

// ---- Array helpers for TLV --------------------------------------------------
inline void tlv_write_vec_u64(TlvWriter& w, FieldId id, const std::vector<std::uint64_t>& v)
{ 
    spd::MemoryOStream mos; write_varuint32(mos, (std::uint32_t)v.size());
    for (auto h : v) 
        mos.write_exact(&h, sizeof(h));
    w.bytes(id, mos.buffer());
}

// Add a convenience method on TlvReader to read vector<u64>
// Value encoding: [varuint32 count][count * u64]
inline std::vector<std::uint64_t> tlv_read_vec_u64(TlvReader& r, const TlvReader::Item& it)
{ 
    std::vector<std::uint8_t> buf = r.read_bytes(it);
    spd::MemoryIStream is(std::move(buf));
    std::uint32_t count = read_varuint32(is);
    std::vector<std::uint64_t> out; out.resize(count);
    for (std::uint32_t i=0;i<count;++i) 
    { 
        is.read_exact(&out[i], sizeof(std::uint64_t));
    }
    return out;
}

// ---- Qt QPoint helpers (optional) -------------------------------------------
#include <QPoint>
#include <QVector>

// Encoding for QVector<QPoint> or std::vector<QPoint>:
//   [varuint32 count][ count * (int32 x, int32 y) ]
inline void tlv_write_vec_qpoint(TlvWriter& w, FieldId id, const QVector<QPoint>& pts)
{ 
    spd::MemoryOStream mos; write_varuint32(mos, (std::uint32_t)pts.size());
    for (const QPoint& p : pts) 
    { 
        std::int32_t x = p.x(); std::int32_t y = p.y();
        mos.write_exact(&x, sizeof(x));
        mos.write_exact(&y, sizeof(y));
    }
    w.bytes(id, mos.buffer());
}

inline void tlv_write_vec_qpoint(TlvWriter& w, FieldId id, const std::vector<QPoint>& pts)
{ 
    spd::MemoryOStream mos; write_varuint32(mos, (std::uint32_t)pts.size());
    for (const QPoint& p : pts) 
    { 
        std::int32_t x = p.x(); std::int32_t y = p.y();
        mos.write_exact(&x, sizeof(x));
        mos.write_exact(&y, sizeof(y));
    }
    w.bytes(id, mos.buffer());
}

// (Optional) readers to reconstruct vectors of QPoint
inline QVector<QPoint> tlv_read_vec_qpoint_qvector(TlvReader& r, const TlvReader::Item& it)
{ 
    std::vector<std::uint8_t> buf = r.read_bytes(it);
    spd::MemoryIStream is(std::move(buf));
    std::uint32_t n = read_varuint32(is);
    QVector<QPoint> out; out.resize((int)n);
    for (std::uint32_t i=0;i<n;++i)
    { 
        std::int32_t x{}, y{}; 
        is.read_exact(&x,sizeof(x)); 
        is.read_exact(&y,sizeof(y)); 
        out[(int)i] = QPoint(x,y); 
    }
    return out;
}
inline std::vector<QPoint> tlv_read_vec_qpoint_std(TlvReader& r, const TlvReader::Item& it)
{ 
    std::vector<std::uint8_t> buf = r.read_bytes(it);
    spd::MemoryIStream is(std::move(buf));
    std::uint32_t n = read_varuint32(is);
    std::vector<QPoint> out; out.reserve(n);
    for (std::uint32_t i=0;i<n;++i)
    { 
        std::int32_t x{}, y{}; 
        is.read_exact(&x,sizeof(x)); 
        is.read_exact(&y,sizeof(y)); 
        out.emplace_back(x,y); 
    }
    return out;
}