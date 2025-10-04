// ============================================================================
// spd_runtime.hpp  — concrete streams, container, TLV, registry, shapes
// ============================================================================
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <unordered_map>
#include <optional>
#include <memory>
#include <iostream>

#include "spd_core.hpp"

namespace spd {

// ============================= CRC32 =========================================
class CRC32 
{
public: 
    CRC32();
    void reset();
    void update(const void* d, std::size_t n);
    std::uint32_t value() const;
private: 
    std::uint32_t crc_{};
};

// ============================ FileHeader =====================================
struct FileHeader final
{
    std::uint32_t magic = make_fourcc('S','P','D','F');
    std::uint16_t major = 1, minor = 0;
    std::uint8_t  endian = 1, reserved = 0;
    std::uint16_t hdrSize = sizeof(FileHeader);
    std::uint64_t dirOffset = 0; // offset of directory array<ChunkDesc>
    std::uint32_t dirCount  = 0; // number of ChunkDesc entries
    std::uint32_t headerCrc = 0; // CRC of header with this field zeroed
    static constexpr std::uint32_t MAGIC = make_fourcc('S','P','D','F');

    void write(OStream& os) const;
    static FileHeader read(IStream& is);
};

// ============================= Concrete Streams ==============================
class FileIStream final : public IStream 
{
public: 
    explicit FileIStream(const std::string& path);
    std::size_t read(void* d, std::size_t n) override;
    void seek(std::int64_t off, SeekWhence w) override;
    std::uint64_t tell() const override;
    std::uint64_t size() const override;
private: 
    std::ifstream f_; 
    std::uint64_t size_{}; 
};

class FileOStream final : public OStream 
{ 
public: 
    explicit FileOStream(const std::string& path);
    std::size_t write(const void* s, std::size_t n) override;
    void seek(std::int64_t off, SeekWhence w) override;
    std::uint64_t tell() const override;
    void flush() override;
private: 
    std::ofstream f_; 
};

class MemoryIStream final : public IStream 
{ 
public: 
    explicit MemoryIStream(std::vector<std::uint8_t> buf);
    std::size_t read(void* d, std::size_t n) override;
    void seek(std::int64_t off, SeekWhence w) override;
    std::uint64_t tell() const override;
    std::uint64_t size() const override;
    const std::vector<std::uint8_t>& buffer() const;
private: 
    std::vector<std::uint8_t> buf_; 
    std::size_t pos_ = 0; 
};

class MemoryOStream final : public OStream 
{ 
public: 
    std::size_t write(const void* s, std::size_t n) override;
    void seek(std::int64_t off, SeekWhence w) override;
    std::uint64_t tell() const override;
    void flush() override;
    const std::vector<std::uint8_t>& buffer() const;
    std::vector<std::uint8_t>& buffer();
private: 
    std::vector<std::uint8_t> buf_; 
    std::size_t pos_ = 0; 
};

// ============================= Directory I/O ================================
void write_chunk_desc(OStream& os, const ChunkDesc& cd);
ChunkDesc read_chunk_desc(IStream& is);

// =========================== Container Writer ===============================
struct PendingChunk 
{ 
    FourCC type{}; 
    Compression comp{Compression::None}; 
    std::vector<std::uint8_t> payload; 
};

class ContainerWriter 
{ 
public: 
    explicit ContainerWriter(OStream& out);
    void addChunk(FourCC t, const void* data, std::size_t n, Compression c = Compression::None);
    void addChunk(FourCC t, const std::vector<std::uint8_t>& v, Compression c = Compression::None);
    void finalize();
private: 
    OStream& out_; 
    std::vector<PendingChunk> chunks_; 
};

// =========================== Container Reader ===============================
class ContainerReader 
{ 
public: 
    explicit ContainerReader(IStream& in);
    const FileHeader& header() const;
    const std::vector<ChunkDesc>& directory() const;
    std::optional<std::size_t> find(FourCC type) const;
    std::vector<std::uint8_t> readPayload(std::size_t idx, bool verifyCrc = true) const;
private: 
    IStream& in_; 
    FileHeader header_{}; 
    std::vector<ChunkDesc> dir_; 
};

}
