#ifndef WORKING_SET_HPP
#define WORKING_SET_HPP

#include "basic_shape.hpp"

#include <vector>
#include <QObject>

class runtime_environment;

class working_set : public QObject
{
        Q_OBJECT

public:
        void clear();

        void add_object(IShape*);

        void add_object(runtime_environment*);

        std::vector<IShape*> get_objects();
private:
        std::vector<IShape*> shapes;
};

#endif
