#include "design.hpp"
#include "spd_runtime.hpp"
#include "tlv.hpp"
#include <stdexcept>
#include <cstring>

// IObjectPool interface implementation
std::string Design::getName() const noexcept
{
    return "Design";
}

IShapePtr Design::addObject(IShapePtr s)
{
    auto obj = std::dynamic_pointer_cast<DesignObject>(s);
    if (obj)
    {
        addObject(obj);
    }
    return s;
}

void Design::removeObject(IShapePtr s) noexcept
{
    auto obj = std::dynamic_pointer_cast<DesignObject>(s);
    if (obj)
    {
        removeObject(obj);
    }
}

void Design::clear() noexcept
{
    objects.clear();
}

std::vector<IShapePtr> Design::getObjects() const noexcept
{
    std::vector<IShapePtr> result;
    result.reserve(objects.size());
    for (const auto& pair : objects)
    {
        auto obj = std::dynamic_pointer_cast<IShape>(pair.second);
        if (obj)
        {
            result.push_back(obj);
        }
    }
    return result;
}

void Design::dumpToFile(const std::string&) const
{
    // Implementation would serialize to file
    // For now, placeholder
}

// Design interface implementation
Handle Design::addObject(DesignObjectPtr s)
{
    if (!s) {
        throw std::invalid_argument("Cannot add null DesignObject");
    }
    
    // Generate a new handle/ID
    static Handle nextHandle = 1;
    Handle h = nextHandle++;
    
    s->id = h;
    objects[h] = s;
    return h;
}

void Design::removeObject(DesignObjectPtr s) noexcept
{
    if (!s) return;
    
    auto it = objects.find(s->id);
    if (it != objects.end()) {
        objects.erase(it);
    }
}

void Design::removeObject(Handle id) noexcept
{
    auto it = objects.find(id);
    if (it != objects.end()) {
        objects.erase(it);
    }
}

std::vector<Handle> Design::getObjectIDs() const noexcept
{
    std::vector<Handle> result;
    result.reserve(objects.size());
    for (const auto& pair : objects) {
        result.push_back(pair.first);
    }
    return result;
}

DesignObjectPtr Design::getObject(Handle id) const noexcept
{
    auto it = objects.find(id);
    if (it != objects.end()) {
        return it->second;
    }
    return nullptr;
}

// Serialization methods
void Design::write(spd::ContainerWriter& cw) const
{
    // Build OBJT payload
    spd::MemoryOStream mos; 
    std::vector<IndexEntry> idx; 
    idx.reserve(objects.size());
    
    for (const auto& kv : objects) 
    {
        const DesignObject& o = *kv.second;
        const std::uint64_t recOff = mos.tell();

        // Write header (except tlv length which is varuint32 after TLV prepared)
        // We'll write ver+guid+handles, then TLV as buffer preceded by varuint32 len.
        std::uint8_t ver = 1; 
        mos.write_exact(&ver, 1);
        mos.write_exact(o.guid().data(), o.guid().size());
        mos.write_exact(&o.id, sizeof(o.id));
        mos.write_exact(&o.owner, sizeof(o.owner));

        // Prepare TLV in a temp buffer
        spd::MemoryOStream tlvBuf; 
        TlvWriter tw(tlvBuf);
        o.writeTlv(tw);
        const auto& blob = tlvBuf.buffer();
        write_varuint32(mos, static_cast<std::uint32_t>(blob.size()));
        if (!blob.empty()) 
            mos.write_exact(blob.data(), blob.size());

        const std::uint64_t recLen = mos.tell() - recOff;
        idx.push_back(IndexEntry{ o.id, recOff, static_cast<std::uint32_t>(recLen) });
    }

    // Emit chunks
    cw.addChunk(spd::ChunkType::OBJT, mos.buffer());
    // Serialize index as plain array
    cw.addChunk(spd::ChunkType::INDX, idx.data(), idx.size() * sizeof(IndexEntry));
}

void Design::read(spd::ContainerReader& cr)
{
    objects.clear();
    auto objIdx = cr.find(spd::ChunkType::OBJT);
    if (!objIdx) return;
    
    auto objBytes = cr.readPayload(*objIdx);
    spd::MemoryIStream mis(std::move(objBytes));

    // Build offset map if INDX exists
    std::unordered_map<Handle, IndexEntry> indexMap;
    if (auto idx = cr.find(spd::ChunkType::INDX)) 
    {
        auto idxBytes = cr.readPayload(*idx);
        const std::size_t n = idxBytes.size() / sizeof(IndexEntry);
        indexMap.reserve(n);
        for (std::size_t i = 0; i < n; ++i)
        {
            IndexEntry e{}; 
            std::memcpy(&e, idxBytes.data() + i * sizeof(IndexEntry), sizeof(IndexEntry));
            indexMap[e.handle] = e;
        }
    }

    // Linear scan if no index
    while (mis.tell() < mis.size()) 
    {
        // Parse record
        std::uint8_t ver{}; 
        mis.read_exact(&ver, 1); 
        if (ver != 1) 
            throw std::runtime_error("OBJT: bad version");
        
        ClassGuid g{}; 
        mis.read_exact(g.data(), g.size());
        Handle h{}; 
        mis.read_exact(&h, sizeof(h));
        Handle owner{}; 
        mis.read_exact(&owner, sizeof(owner));
        std::uint32_t tlvLen = read_varuint32(mis);
        const std::uint64_t tlvStart = mis.tell();
        const std::uint64_t tlvEnd = tlvStart + tlvLen;

        // Instantiate via registry
        auto obj = Registry::instance().create(g);
        if (!obj)
        {
            // Unknown type: skip TLV
            mis.seek(static_cast<std::int64_t>(tlvLen), spd::SeekWhence::Current);
            continue;
        }
        obj->id = h; 
        obj->owner = owner;

        // TLV reader spans current window
        TlvReader tr(mis, tlvStart, tlvEnd);
        // For generality, you'd call obj->readTlv(tr) and ensure it can read.
        obj->readTlv(tr);
        mis.seek(static_cast<std::int64_t>(tlvEnd), spd::SeekWhence::Begin);

        objects[h] = std::static_pointer_cast<DesignObject>(obj);
    }
}

// Template method implementation
template<class T, class...Args>
T* Design::emplace(Handle h, Args&&...args) {
    auto p = std::make_shared<T>(std::forward<Args>(args)...);
    p->id = h; 
    auto* raw = p.get(); 
    objects[h] = std::move(p); 
    return raw;
}

void Design::loadFromFile(const std::string& filename)
{   
    spd::FileIStream fis(filename);
    spd::ContainerReader cr(fis);
    read(cr);
}

void Design::saveToFile(const std::string& filename) const
{
    spd::FileOStream fos(filename);
    spd::ContainerWriter cw(fos);
    write(cw);
    cw.finalize();
}
