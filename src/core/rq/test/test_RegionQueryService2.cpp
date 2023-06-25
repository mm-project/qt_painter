#include "../../ishape.hpp"
#include "../../qt_shapes/rectangle.hpp"
#include "../../qt_shapes/ellipse.hpp"
#include "../../qt_shapes/line.hpp"
#include "../../qt_shapes/polygon.hpp"
#include "../RegionQueryService.hpp"
#include "../debug_helper.hpp"
#include "../../../commands/qa_commands.hpp"

#include <QPoint>
#include <QPainter>
#include <QPixmap>
#include <QGuiApplication>


#include <cassert>
#include <iostream>
#include <filesystem>

template <typename T> IShapePtr create(QPoint p1, QPoint p2)
{
    IShapePtr shape = std::shared_ptr<T>(new T);
    shape->addPoint(p2);
    shape->addPoint(p1);

    return shape;
}

void create_rect(int x1, int y1, int x2, int y2)
{
    RegionQuery &rq = RegionQuery::getInstance();
    rq.insertObject(create<Rectangle>(QPoint(x1, y1), QPoint(x2, y2)));
}


void create_rect_at_given_cell_and_row(int column, int row)
{
    int delta = 100;
    int epsilon = 90;
    //std::cout << row << " " << column << std::endl;
    create_rect(delta*row,delta*column,delta*row-epsilon,delta*column-epsilon);
}


bool test_rq_by_inserting_matrix_of_n_objs(int n)
{

    for(int column=0; column<n; column++)
        for(int row=0; row<n; row++)
            create_rect_at_given_cell_and_row(column,row);
}


int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    int n = 200;
    std::cout << "inserting " << n << "x" << n << " objects ..." << std::endl;
    test_rq_by_inserting_matrix_of_n_objs(n);


    std::cout << "rendering..." << std::endl;
    QPixmap device(10000,10000);
    device.fill(Qt::black);
    QPainter *painter = new QPainter(&device);

    RegionQuery &rq = RegionQuery::getInstance();
    for (auto & shape: rq.getShapesUnderRect(QRect(0, 0, 1000000, 1000000)))
        shape->draw(painter);

    device.save("rq.png");

    std::cout << "comparing..." << std::endl;
    assert(are_imagefiles_different("rq.png","expected/rq.png"));

    delete painter;

    std::cout << "pass" << std::endl;


}
