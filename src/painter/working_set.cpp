#include "working_set.hpp"

#include "runtime_environment.hpp"

void working_set::add_object(basic_shape* s)
{
        shapes.push_back(s->clone());
}

std::vector<basic_shape*> working_set::get_objects()
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
