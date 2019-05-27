#include "working_set.hpp"

void WorkingSet::addObject(IShape* s)
{
        m_shapes.push_back(s->clone());
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

