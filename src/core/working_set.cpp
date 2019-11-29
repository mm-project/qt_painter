#include "working_set.hpp"

#include <QFile>
#include <QDir>
#include <QTextStream>

#include <cassert>

IShape* WorkingSet::addObject(IShape* s)
{
	assert(s);
    auto obj = s->clone();
	assert(obj);
    m_shapes.push_back(obj);
	return obj;
}
 
std::vector<IShape*> WorkingSet::getObjects() const
{
    return m_shapes;
}

void WorkingSet::clear()
{
	for (auto i : m_shapes)
		delete i;
	m_shapes.clear();
}

std::string WorkingSet::getName()
{
    return "Design";
}

void WorkingSet::dumpToFile(const std::string& fname)
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

void WorkingSet::removeObject(IShape* obj)
{
	auto it = std::find(m_shapes.begin(), m_shapes.end(), obj);
	if (it == m_shapes.end())
		return;
	m_shapes.erase(it);
	delete obj;
}
