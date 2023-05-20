#include "../RegionQueryService.hpp"
#include "../debug_helper.hpp"

#include <QPoint>

#include <cassert>
#include <iostream>


//std::map<int, IShapePtr> index2shape;
//int shapes_num = 0;

template <typename T> IShapePtr create(QPoint p1, QPoint p2)
{
    IShapePtr shape = std::shared_ptr<T>(new T);
    // std::cout << shapes_num << " " << shape << std::endl;
    //index2shape[++shapes_num] = shape;
    shape->addPoint(p2);
    shape->addPoint(p1);
    // if ( std::is_same<T, polygon>)
    //    shape->addPoint(p3);

    return shape;
}

void create_rect(int x1, int y1, int x2, int y2)
{
    RegionQuery &rq = RegionQuery::getInstance();
    rq.insertObject(create<Rectangle>(QPoint(x1, y1), QPoint(x2, y2))); // n1
}


void create_rect_at_given_cell_and_row(int column, int row)
{
    int delta = 100;
    int epsilon = 90;
    create_rect(delta*row,delta*column,delta*row-epsilon,delta*column-epsilon);
}


bool test_rq_by_inserting_1k_by_1k_objs()
{

    for(int column=0; column<1000; column++)
        for(int row=0; row<1000; row++)
            create_rect(0,0,100,100);
            //create_rect_at_given_cell_and_row(column,row);

}


int main(int argc, char **argv)
{
    test_rq_by_inserting_1k_by_1k_objs();
    std::cout << "pass" << std::endl;
}
