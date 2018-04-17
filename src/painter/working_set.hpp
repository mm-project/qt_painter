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

        void add_object(basic_shape*);

        void add_object(runtime_environment*);

        std::vector<basic_shape*> get_objects();
private:
        std::vector<basic_shape*> shapes;
};

#endif
