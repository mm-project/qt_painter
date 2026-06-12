#include "tlv.hpp"

// VarUInt32 (LEB128) implementations
void write_varuint32(spd::OStream& os, std::uint32_t v) 
{
    while (v >= 0x80u) { 
        std::uint8_t b = (std::uint8_t)(v | 0x80u); 
        os.write_exact(&b, 1); 
        v >>= 7;
    } 
    std::uint8_t last = (std::uint8_t)v; 
    os.write_exact(&last, 1);
}

std::uint32_t read_varuint32(spd::IStream& is) 
{
    std::uint32_t r = 0; 
    int sh = 0; 
    for (;;) { 
        std::uint8_t b{}; 
        is.read_exact(&b, 1); 
        r |= (std::uint32_t)(b & 0x7Fu) << sh; 
        if ((b & 0x80u) == 0) 
            break; 
        sh += 7; 
        if (sh > 28) 
            throw std::runtime_error("varuint32 overflow"); 
    } 
    return r; 
}

// TlvWriter implementation
TlvWriter::TlvWriter(spd::OStream& os) : os_(os) 
{
}

void TlvWriter::field(FieldId id, const void* data, std::size_t len) 
{
    os_.write_exact(&id, sizeof(id)); 
    write_varuint32(os_, (std::uint32_t)len); 
    if (len) 
        os_.write_exact(data, len);
}

void TlvWriter::u8(FieldId id, std::uint8_t v) 
{ 
    field(id, &v, sizeof(v)); 
}

void TlvWriter::u16(FieldId id, std::uint16_t v) 
{ 
    field(id, &v, sizeof(v)); 
}

void TlvWriter::u32(FieldId id, std::uint32_t v) 
{ 
    field(id, &v, sizeof(v)); 
}

void TlvWriter::u64(FieldId id, std::uint64_t v) 
{ 
    field(id, &v, sizeof(v)); 
}

void TlvWriter::f32(FieldId id, float v) 
{ 
    field(id, &v, sizeof(v)); 
}

void TlvWriter::f64(FieldId id, double v) 
{ 
    field(id, &v, sizeof(v)); 
}

void TlvWriter::bytes(FieldId id, const std::vector<std::uint8_t>& v) 
{ 
    field(id, v.data(), v.size()); 
}

void TlvWriter::str(FieldId id, const std::string& s) 
{ 
    field(id, s.data(), s.size()); 
}

// TlvReader implementation
TlvReader::TlvReader(spd::IStream& is, std::uint64_t start, std::uint64_t end) 
    : is_(is), cur_(start), end_(end) 
{
}

bool TlvReader::next(Item& out) 
{
    if (cur_ >= end_) 
        return false; 
    is_.seek((std::int64_t)cur_, spd::SeekWhence::Begin); 
    FieldId id{}; 
    is_.read_exact(&id, sizeof(id)); 
    std::uint32_t len = read_varuint32(is_); 
    std::uint64_t valOff = is_.tell(); 
    out = Item{ id, len, valOff }; 
    cur_ = valOff + len; 
    return true; 
}

std::string TlvReader::read_string(const Item& it) 
{
    std::string s; 
    s.resize(it.len); 
    if (it.len) {
        is_.seek((std::int64_t)it.valueOffset, spd::SeekWhence::Begin); 
        is_.read_exact(s.data(), it.len);
    } 
    return s; 
}

std::vector<std::uint8_t> TlvReader::read_bytes(const Item& it) 
{
    std::vector<std::uint8_t> v(it.len); 
    if (it.len) {
        is_.seek((std::int64_t)it.valueOffset, spd::SeekWhence::Begin); 
        is_.read_exact(v.data(), it.len);
    } 
    return v; 
}

void TlvReader::read_raw(const Item& it, void* dst, std::size_t n) 
{
    if (n != it.len) 
        throw std::runtime_error("TLV size"); 
    is_.seek((std::int64_t)it.valueOffset, spd::SeekWhence::Begin); 
    is_.read_exact(dst, n); 
}
