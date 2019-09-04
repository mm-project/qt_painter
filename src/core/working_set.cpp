#include "working_set.hpp"
#include "postman.hpp"
#include "rq/RegionQueryService.hpp"

#include <QFile>
#include <QDir>
#include <QTextStream>

//WorkingSet::WorkingSet() {
//
//}

void WorkingSet::addObject(IShape* s)
{
    cb = new WSCallbackData;
    IShape* c = s->clone();    
    m_shapes.push_back(c);
    m_clone2original[c] = s;
    
    if ( s->m_rt_mode == false ) {
        cb->set_data(c);
        NOTIFY(DB_SHAPE_ADDED,*cb);
    }
}

//bool WorkingSet::isModified() {
//    return m_modified;
//}

IShape* WorkingSet::getClonee(IShape* s) 
{
    return m_clone2original[s];
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
    //WSCallbackData d(obj);
    //NOTIFY(DB_SHAPE_WILLBE_DELETED,d);
    delete obj;
    obj=0;
}
