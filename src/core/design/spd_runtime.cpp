#include "spd_runtime.hpp"

namespace spd {

// ============================= CRC32 Implementation ==========================
CRC32::CRC32() 
{ 
    reset(); 
}

void CRC32::reset() 
{ 
    crc_ = 0xFFFFFFFFu; 
}

void CRC32::update(const void* d, std::size_t n) 
{
    auto* p = (const std::uint8_t*)d;
    for (size_t i = 0; i < n; ++i) {
        crc_ ^= p[i];
        for (int k = 0; k < 8; ++k) {
            std::uint32_t m = -(crc_ & 1u);
            crc_ = (crc_ >> 1) ^ (0xEDB88320u & m);
        }
    }
}

std::uint32_t CRC32::value() const 
{ 
    return crc_ ^ 0xFFFFFFFFu; 
}

// ============================= FileHeader Implementation ===================
void FileHeader::write(OStream& os) const 
{
    FileHeader tmp = *this; 
    tmp.headerCrc = 0; 
    CRC32 c; 
    c.update(&tmp, sizeof(tmp)); 
    tmp.headerCrc = c.value();
    os.seek(0, SeekWhence::Begin); 
    os.write_exact(&tmp, sizeof(tmp));
}

FileHeader FileHeader::read(IStream& is)
{
    FileHeader h{}; 
    is.seek(0, SeekWhence::Begin); 
    is.read_exact(&h, sizeof(h));
    if (h.magic != MAGIC) 
        throw std::runtime_error("SPD: bad magic"); 
    FileHeader chk = h; 
    chk.headerCrc = 0; 
    CRC32 c; 
    c.update(&chk, sizeof(chk)); 
    if (c.value() != h.headerCrc) 
        throw std::runtime_error("SPD: header CRC"); 
    if (h.endian != 1) 
        throw std::runtime_error("SPD: big-endian not supported"); 
    return h; 
}

// ============================= FileIStream Implementation ==================
FileIStream::FileIStream(const std::string& path) 
    : f_(path, std::ios::binary | std::ios::in)
{ 
    if (!f_) 
        throw std::runtime_error("open read: " + path); 
    f_.seekg(0, std::ios::end); 
    size_ = (std::uint64_t)f_.tellg(); 
    f_.seekg(0, std::ios::beg);
}

std::size_t FileIStream::read(void* d, std::size_t n) 
{ 
    f_.read((char*)d, (std::streamsize)n); 
    return (std::size_t)f_.gcount(); 
}

void FileIStream::seek(std::int64_t off, SeekWhence w) 
{ 
    std::ios::seekdir dir = std::ios::beg; 
    if (w == SeekWhence::Current) 
        dir = std::ios::cur; 
    else if (w == SeekWhence::End) 
        dir = std::ios::end; 
    f_.clear(); 
    f_.seekg(off, dir);
}

std::uint64_t FileIStream::tell() const 
{ 
    return (std::uint64_t)const_cast<std::ifstream&>(f_).tellg(); 
}

std::uint64_t FileIStream::size() const 
{ 
    return size_; 
}

// ============================= FileOStream Implementation ==================
FileOStream::FileOStream(const std::string& path) 
    : f_(path, std::ios::binary | std::ios::out | std::ios::trunc)
{ 
    if (!f_) 
        throw std::runtime_error("open write: " + path);
}

std::size_t FileOStream::write(const void* s, std::size_t n) 
{ 
    f_.write((const char*)s, (std::streamsize)n); 
    return f_ ? n : 0; 
}

void FileOStream::seek(std::int64_t off, SeekWhence w) 
{ 
    std::ios::seekdir dir = std::ios::beg; 
    if (w == SeekWhence::Current) 
        dir = std::ios::cur; 
    else if (w == SeekWhence::End) 
        dir = std::ios::end; 
    f_.clear(); 
    f_.seekp(off, dir);
}

std::uint64_t FileOStream::tell() const 
{ 
    return (std::uint64_t)const_cast<std::ofstream&>(f_).tellp(); 
}

void FileOStream::flush() 
{ 
    f_.flush(); 
}

// ============================= MemoryIStream Implementation ================
MemoryIStream::MemoryIStream(std::vector<std::uint8_t> buf) 
    : buf_(std::move(buf))
{
}

std::size_t MemoryIStream::read(void* d, std::size_t n) 
{ 
    const auto rem = buf_.size() - pos_; 
    const auto to = rem < n ? rem : n; 
    if (to) 
        std::memcpy(d, buf_.data() + pos_, to); 
    pos_ += to; 
    return to; 
}

void MemoryIStream::seek(std::int64_t off, SeekWhence w) 
{ 
    std::int64_t base = 0; 
    if (w == SeekWhence::Current) 
        base = (std::int64_t)pos_; 
    else if (w == SeekWhence::End) 
        base = (std::int64_t)buf_.size(); 
    auto np = base + off; 
    if (np < 0 || (std::size_t)np > buf_.size()) 
        throw std::runtime_error("MemoryIStream seek"); 
    pos_ = (std::size_t)np; 
}

std::uint64_t MemoryIStream::tell() const 
{ 
    return pos_; 
}

std::uint64_t MemoryIStream::size() const 
{ 
    return buf_.size(); 
}

const std::vector<std::uint8_t>& MemoryIStream::buffer() const 
{ 
    return buf_; 
}

// ============================= MemoryOStream Implementation ================
std::size_t MemoryOStream::write(const void* s, std::size_t n) 
{ 
    const auto* p = (const std::uint8_t*)s; 
    if (pos_ + n > buf_.size()) 
        buf_.resize(pos_ + n); 
    std::memcpy(buf_.data() + pos_, p, n); 
    pos_ += n; 
    return n; 
}

void MemoryOStream::seek(std::int64_t off, SeekWhence w) 
{ 
    std::int64_t base = 0; 
    if (w == SeekWhence::Current) 
        base = (std::int64_t)pos_; 
    else if (w == SeekWhence::End) 
        base = (std::int64_t)buf_.size(); 
    auto np = base + off; 
    if (np < 0) 
        throw std::runtime_error("MemoryOStream neg seek"); 
    pos_ = (std::size_t)np; 
    if (pos_ > buf_.size()) 
        buf_.resize(pos_); 
}

std::uint64_t MemoryOStream::tell() const 
{ 
    return pos_; 
}

void MemoryOStream::flush() 
{
}

const std::vector<std::uint8_t>& MemoryOStream::buffer() const 
{ 
    return buf_; 
}

std::vector<std::uint8_t>& MemoryOStream::buffer() 
{ 
    return buf_; 
}

// ============================= Directory I/O Implementation ===============
void write_chunk_desc(OStream& os, const ChunkDesc& cd)
{ 
    os.write_exact(&cd, sizeof(cd));
}

ChunkDesc read_chunk_desc(IStream& is)
{ 
    ChunkDesc cd{}; 
    is.read_exact(&cd, sizeof(cd)); 
    return cd; 
}

// ============================= ContainerWriter Implementation ==============
ContainerWriter::ContainerWriter(OStream& out) 
    : out_(out)
{ 
    FileHeader h{}; 
    h.write(out_);
}

void ContainerWriter::addChunk(FourCC t, const void* data, std::size_t n, Compression c)
{ 
    PendingChunk pc; 
    pc.type = t; 
    pc.comp = c; 
    pc.payload.resize(n); 
    if (n) 
        std::memcpy(pc.payload.data(), data, n); 
    chunks_.push_back(std::move(pc)); 
}

void ContainerWriter::addChunk(FourCC t, const std::vector<std::uint8_t>& v, Compression c)
{ 
    addChunk(t, v.data(), v.size(), c);
}

void ContainerWriter::finalize()
{ 
    std::vector<ChunkDesc> dir; 
    dir.reserve(chunks_.size()); 
    for (const auto& pc : chunks_) {
        ChunkDesc cd{}; 
        cd.type = pc.type; 
        cd.comp = pc.comp; 
        cd.offset = out_.tell(); 
        cd.length = pc.payload.size(); 
        CRC32 crc; 
        if (!pc.payload.empty()) 
            crc.update(pc.payload.data(), pc.payload.size()); 
        cd.crc32 = crc.value(); 
        if (!pc.payload.empty()) 
            out_.write_exact(pc.payload.data(), pc.payload.size()); 
        dir.push_back(cd);
    } 
    const std::uint64_t dirOff = out_.tell(); 
    for (const auto& cd : dir) 
        write_chunk_desc(out_, cd); 
    FileHeader hdr{}; 
    hdr.dirOffset = dirOff; 
    hdr.dirCount = (std::uint32_t)dir.size(); 
    hdr.write(out_); 
    out_.flush(); 
}

// ============================= ContainerReader Implementation ==============
ContainerReader::ContainerReader(IStream& in) 
    : in_(in)
{ 
    header_ = FileHeader::read(in_); 
    in_.seek((std::int64_t)header_.dirOffset, SeekWhence::Begin); 
    dir_.resize(header_.dirCount); 
    for (std::uint32_t i = 0; i < header_.dirCount; ++i) 
        dir_[i] = read_chunk_desc(in_);
}

const FileHeader& ContainerReader::header() const 
{ 
    return header_; 
}

const std::vector<ChunkDesc>& ContainerReader::directory() const 
{ 
    return dir_; 
}

std::optional<std::size_t> ContainerReader::find(FourCC type) const 
{ 
    for (size_t i = 0; i < dir_.size(); ++i) 
        if (dir_[i].type == type) 
            return i; 
    return std::nullopt; 
}

std::vector<std::uint8_t> ContainerReader::readPayload(std::size_t idx, bool verifyCrc) const 
{ 
    if (idx >= dir_.size()) 
        throw std::out_of_range("chunk index"); 
    const auto& cd = dir_[idx]; 
    if (cd.comp != Compression::None) 
        throw std::runtime_error("compressed chunks not implemented"); 
    std::vector<std::uint8_t> buf((size_t)cd.length); 
    if (!buf.empty()) {
        in_.seek((std::int64_t)cd.offset, SeekWhence::Begin); 
        in_.read_exact(buf.data(), buf.size()); 
        if (verifyCrc) {
            CRC32 crc; 
            crc.update(buf.data(), buf.size()); 
            if (crc.value() != cd.crc32) 
                throw std::runtime_error("chunk CRC mismatch"); 
        } 
    } 
    return buf; 
}

} // namespace spd
