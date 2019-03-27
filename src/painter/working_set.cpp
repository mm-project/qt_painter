#include "working_set.hpp"

#include "runtime_environment.hpp"

void working_set::add_object(IShape* s)
{
        shapes.push_back(s->clone());
}

std::vector<IShape*> working_set::get_objects()
{
        return shapes;
}

void working_set::clear()
{
        shapes.clear();
}

void working_set::add_object(runtime_environment* r)
{
        shapes.push_back(r->get_runtime_object()->clone());
}
