#include "design.hpp"
#include "core.hpp"

#include <QFile>
#include <QDir>
#include <QTextStream>


IShapePtr Design::addObject(IShapePtr s)
{
	ASSERT_RETURN_VALUE(s != nullptr, nullptr);
	auto obj = std::shared_ptr<IShape>(s->clone());
	ASSERT_RETURN_VALUE(obj != nullptr, nullptr);
	return ObjectPoolBase::addObject(obj);
}

std::string Design::getName() const noexcept
{
	return std::move("Design");
}

void Design::dumpToFile(const std::string& fname) const
{
    QFile file(fname.c_str());
    file.open( QIODevice::WriteOnly | QIODevice::Append ); 
    QTextStream z(&file);
 
    z << "Name: "   << getName().c_str() ;
    z << "\nObjCount: " << QString::number(getObjects().size());
    z << "\n======\n";
    for (auto i : m_shapes) {
        z << ObjType2String(i->getType()).c_str();
        z << ":"; //i->getPoints();
        z << "\n";
    }
    z << "--------";

    file.flush();
    file.close();
}
