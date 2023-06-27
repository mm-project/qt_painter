#include "iobject_pool.hpp"
#include "core.hpp"
#include "ishape.hpp"

#include <vector>
#include <set>

#include <QDir>
#include <QFile>
#include <QTextStream>

void ObjectPoolBase::clear() noexcept
{
    m_shapes.clear();
}

IShapePtr ObjectPoolBase::addObject(IShapePtr shape)
{
    ASSERT_RETURN_VALUE(shape != nullptr, nullptr);
    m_shapes.emplace_back(shape);
    return shape;
}

void ObjectPoolBase::removeObject(IShapePtr shape) noexcept
{
    auto it = std::find(m_shapes.begin(), m_shapes.end(), shape);
    if (it == m_shapes.end())
        return;
    m_shapes.erase(it);
}

std::vector<IShapePtr> ObjectPoolBase::getObjects() const noexcept
{
    return m_shapes;
}

void ObjectPoolBase::dumpToFile(const std::string &fname) const
{
    QFile file(fname.c_str());
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream z(&file);

    // Sort objects before dumping.
    std::vector<std::multiset<ShapeProperties>> shapes_sorted_info(4);
    for (auto i : getObjects())
        shapes_sorted_info[i->getType()].insert(i->getProperties());

    z << "PoolName: " << getName().c_str();
    z << "\nObjCount: " << QString::number(getObjects().size());
    z << "\n======\n";
    for(int i=0; i<shapes_sorted_info.size(); i++)
        for (auto const& y : shapes_sorted_info[i])
        {
            z << ObjType2String(ObjectType(i)).c_str();
            z << ":\t"; // i->getPoints();
            z << y.toString().c_str();
            z << "\n";
        }
    z << "--------";
    z << "\n\n";

    file.flush();
    file.close();
}
