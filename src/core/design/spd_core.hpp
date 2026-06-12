// ============================================================================
// spd_core.hpp — core types, abstract streams, FourCC helpers
// ============================================================================
#pragma once
#include <cstdint>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <array>


namespace spd {

// ---- FourCC -----------------------------------------------------------------
using FourCC = std::uint32_t;
constexpr FourCC make_fourcc(char a, char b, char c, char d) noexcept 
{
    return (static_cast<FourCC>(static_cast<unsigned char>(a)) ) |
    (static_cast<FourCC>(static_cast<unsigned char>(b)) << 8) |
    (static_cast<FourCC>(static_cast<unsigned char>(c)) << 16) |
    (static_cast<FourCC>(static_cast<unsigned char>(d)) << 24);
}


namespace ChunkType 
{
    inline constexpr FourCC OBJT = make_fourcc('O','B','J','T');
    inline constexpr FourCC INDX = make_fourcc('I','N','D','X');
    inline constexpr FourCC META = make_fourcc('M','E','T','A');
}


// ---- Compression tag (placeholder) -----------------------------------------
enum class Compression : std::uint8_t { None = 0, LZ4 = 1, Zstd = 2, Deflate = 3 };


// ---- Chunk descriptor --------------------------------------------------------
struct ChunkDesc final
{
    FourCC type = 0;
    std::uint64_t offset = 0; // payload offset
    std::uint64_t length = 0; // uncompressed length
    std::uint32_t crc32 = 0; // CRC-32 of uncompressed payload
    Compression comp = Compression::None;
    std::uint8_t _reserved[3] = {0,0,0};
};


// ---- Streams & helpers ------------------------------------------------------
enum class SeekWhence : std::uint8_t { Begin, Current, End };


class IStream 
{
public:
    virtual ~IStream() = default;
    virtual std::size_t read(void* dst, std::size_t n) = 0;
    virtual void seek(std::int64_t offset, SeekWhence w = SeekWhence::Begin) = 0;
    virtual std::uint64_t tell() const = 0;
    virtual std::uint64_t size() const = 0;

    void read_exact(void* dst, std::size_t n) 
    {
        std::size_t got = 0; auto* p = static_cast<std::uint8_t*>(dst);
        while (got < n) 
        {
            const auto r = read(p + got, n - got);
            if (r == 0) throw std::runtime_error("IStream: unexpected EOF");
            got += r;
        }
    }
};


class OStream
{
public:
    virtual ~OStream() = default;
    virtual std::size_t write(const void* src, std::size_t n) = 0;
    virtual void seek(std::int64_t offset, SeekWhence w = SeekWhence::Begin) = 0;
    virtual std::uint64_t tell() const = 0;
    virtual void flush() = 0;

    void write_exact(const void* src, std::size_t n) 
    {
        std::size_t put = 0; const auto* p = static_cast<const std::uint8_t*>(src);
        while (put < n) 
        {
            const auto w = write(p + put, n - put);
            if (w == 0) throw std::runtime_error("OStream: short write");
            put += w;
        }
    }
};


// Little-endian primitive helpers (optional)
template <class T>
inline typename std::enable_if<std::is_integral<T>::value, void>::type
write_le(OStream& os, T v)
{
    using U = typename std::make_unsigned<T>::type; U u = static_cast<U>(v);
    for (std::size_t i = 0; i < sizeof(T); ++i) { std::uint8_t b = (u >> (8*i)) & 0xFFu; os.write_exact(&b, 1); }
}


template <class T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
read_le(IStream& is) 
{
    using U = typename std::make_unsigned<T>::type; U u = 0;
    for (std::size_t i = 0; i < sizeof(T); ++i) { std::uint8_t b{}; is.read_exact(&b,1); u |= U(b) << (8*i); }
    return static_cast<T>(u);
}


} // namespace spd
