#include "iobject_pool.hpp"
#include "core.hpp"
#include "ishape.hpp"

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

    z << "PoolName: " << getName().c_str();
    z << "\nObjCount: " << QString::number(getObjects().size());
    z << "\n======\n";
    for (auto i : getObjects())
    {
        z << ObjType2String(i->getType()).c_str();
        z << ":\t"; // i->getPoints();
        z << i->getProperties().toString().c_str();
        z << "\n";
    }
    z << "--------";
    z << "\n\n";

    file.flush();
    file.close();
}
