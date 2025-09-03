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
#include <vector>
#include <chrono>

RegionQuery &rq = RegionQuery::getInstance();
int test_id = 1;
int validation_id = 1;
std::vector<IShapePtr> the_ws;

struct drawble 
{
    QPainter* painter;
    QPixmap* pixmap;
};
std::pair<drawble,drawble> drawbles;

class test_info
{
    public:
        test_info(int ti, int vi) {
            test_id = ti;
            validation_id = vi;
        }
    
    int test_id;
    int validation_id;
};
std::vector<test_info> failed_tests;

auto getShapesUnderRect(int x, int y, int width, int height)
{
    std::vector<IShapePtr> result;

    QRect rect(x, y, width, height);

    for (auto& shape : the_ws) {
        bool inside = false;

        for (const QPoint& p : shape->getPoints()) {
            if (rect.contains(p)) {
                inside = true;
                break;
            }
        }

        if (inside) {
            result.push_back(shape);
        }
    }

    return result;
}

template <typename T> IShapePtr create(QPoint p1, QPoint p2)
{
    IShapePtr shape = std::shared_ptr<T>(new T);
    shape->addPoint(p2);
    shape->addPoint(p1);

    return shape;
}

void create_rect(int x1, int y1, int x2, int y2)
{
    // remove if need to test negative coordinates as well 
    if ( x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 )
        return;

    auto rect = create<Rectangle>(QPoint(x1, y1), QPoint(x2, y2));
    rq.insertObject(rect);
    the_ws.push_back(rect);
}

void create_rect_at_given_cell_and_row(int column, int row)
{
    // don't know, but it does what i wanted
    int delta = 100;
    int epsilon = 90;
    create_rect(delta*row,delta*column,delta*row-epsilon,delta*column-epsilon);
}

void insert_nxn_matrix_of_rects(int n)
{
    std::cout << "  inserting " << n << "x" << n << " objects ..." << std::endl;
    for(int column=0; column<n; column++)
        for(int row=0; row<n; row++)
            create_rect_at_given_cell_and_row(column,row);
}

auto init() 
{
    std::cout <<"[test " << test_id<< "]: initizialazing ..." << std::endl;
    rq.clear();
    
    QPixmap* pixmap1 = new QPixmap(10000,10000);
    pixmap1->fill(Qt::black);
    QPainter* painter1 = new QPainter(pixmap1);

    QPixmap* pixmap2 = new QPixmap(10000,10000);
    pixmap2->fill(Qt::black);
    QPainter* painter2 = new QPainter(pixmap2);

    drawble d1;
    d1.painter = painter1;
    d1.pixmap = pixmap1;

    drawble d2;
    d2.painter = painter2;
    d2.pixmap = pixmap2;

    drawbles = std::make_pair(d1,d2);
}

void fini()
{
    delete drawbles.first.painter;
    delete drawbles.first.pixmap;
    delete drawbles.second.painter;
    delete drawbles.second.pixmap;
    
    drawbles.first.pixmap = 0;
    drawbles.second.pixmap = 0;
    drawbles.first.painter = 0;
    drawbles.second.painter = 0;

    rq.clear();
    test_id++;
    validation_id = 1;
}

void refresh_drawbles()
{
    delete drawbles.first.painter;
    delete drawbles.first.pixmap;
    delete drawbles.second.painter;
    delete drawbles.second.pixmap;

    QPixmap* pixmap1 = new QPixmap(10000,10000);
    pixmap1->fill(Qt::black);
    QPainter* painter1 = new QPainter(pixmap1);

    QPixmap* pixmap2 = new QPixmap(10000,10000);
    pixmap2->fill(Qt::black);
    QPainter* painter2 = new QPainter(pixmap2);

    drawble d1;
    d1.painter = painter1;
    d1.pixmap = pixmap1;

    drawble d2;
    d2.painter = painter2;
    d2.pixmap = pixmap2;

    //return std::pair<drawble,drawble>(d1,d2);
    //return std::pair<QPixmap*,QPainter*>(pixmap,painter);
    drawbles = std::make_pair(d1,d2);
}

void validate_rq(int x, int y, int width, int height)
{
    std::cout <<"  [validation " << validation_id<< "]: proceeding query ..." << std::endl;
    auto rq_start = std::chrono::high_resolution_clock::now();
    auto rq_shapes = rq.getShapesUnderRect(QRect(x, y, width, height));
    auto rq_end = std::chrono::high_resolution_clock::now();
    auto rq_duration = std::chrono::duration_cast<std::chrono::milliseconds>(rq_end - rq_start).count();
    std::cout << "       --> rq lookup took " << rq_duration << "ms and got " << rq_shapes.size() << " objects" << std::endl;
    
    auto ws_start = std::chrono::high_resolution_clock::now();
    auto ws_shapes = getShapesUnderRect(x, y, width, height);
    auto ws_end = std::chrono::high_resolution_clock::now();
    auto ws_duration = std::chrono::duration_cast<std::chrono::milliseconds>(ws_end - ws_start).count();
    std::cout << "       --> ws lookup took " << ws_duration << "ms and got " << ws_shapes.size() << " objects" << std::endl;

    if (rq_shapes.size() != ws_shapes.size()) {
        std::cout << "       Issue found!" << std::endl;
        failed_tests.push_back(test_info(test_id,validation_id));
    } else {
        std::cout << "       passed" << std::endl;
    }
    
    std::cout << "        drawing and saving design data..." << std::endl;
    QPixmap pixmap(10000,10000);
    pixmap.fill(Qt::black);
    QPainter* painter = new QPainter(&pixmap);
    for (auto & shape: the_ws)
        shape->draw(painter);
    auto select_rect = create<Rectangle>(QPoint(x, y), QPoint(x+width, y+height));
    ShapeProperties p;
    p.pen_color = Qt::red;
    p.brush_color = Qt::red;
    select_rect->updateProperties(p);
    select_rect->draw(painter);
    QString fname_des = "des_" + QString::number(test_id) + "_" + QString::number(validation_id) + ".png"; 
    pixmap.save(fname_des);
    delete painter;
    painter = 0;

    std::cout << "        drawing rq data..." << std::endl;
    for (auto & shape: rq_shapes)
        shape->draw(drawbles.first.painter);

    QString fname_rq = "rq_" + QString::number(test_id) + "_" + QString::number(validation_id) + ".png"; 
    std::cout << "        saving rq data to " << fname_rq.toStdString() << " ..." << std::endl;
    drawbles.first.pixmap->save(fname_rq);

    std::cout << "        comparing with expected ..." << std::endl;
    //assert(!are_imagefiles_different("rq.png","expected/rq.png"));

    std::cout << "        drawing ws data..." << std::endl;
    for (auto & shape: ws_shapes)
        shape->draw(drawbles.second.painter);

    QString fname_ws = "ws_" + QString::number(test_id) + "_" + QString::number(validation_id) + ".png"; 
    std::cout << "        saving ws data to " << fname_ws.toStdString() << " ..." << std::endl;
    drawbles.second.pixmap->save(fname_ws);
    
    refresh_drawbles();
    validation_id++;
}

void print_results()
{
    std::cout << std::endl;
    std::cout << "******* R E S U L T *******" << std::endl;
    
    if ( failed_tests.size() != 0 ) {
        for (auto & failed_test: failed_tests) {
            std::cout << "Failed: test" << failed_test.test_id << " in validation " << failed_test.validation_id << std::endl;
        }
        std::cout << std::endl;
        std::cout << "tests failed." << std::endl;
        std::cout << std::endl;
    } else {
        std::cout << std::endl;
        std::cout << "all tests passed." << std::endl;
        std::cout << std::endl;
    }

}


void test1() 
{
    init();
    insert_nxn_matrix_of_rects(1000);
    
    //v1
    validate_rq(0, 0, 10, 10);
    //v2
    validate_rq(0, 0, 500, 500);
    //v3
    validate_rq(0, 0, 1000, 1000);
    //v4
    validate_rq(88, 88, 101, 103);
    //v5
    validate_rq(1900, 1900, 100, 203);
    //v6
    validate_rq(1234, 4321, 10, 999);
    //v7
    validate_rq(1234, 4321, 999, 10);
    //v8
    validate_rq(1111, 2222, 3333, 444);
    
    fini();
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    test1();
    //test2();
    //test3()

    print_results();
}
