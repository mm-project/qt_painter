#include "../../ishape.hpp"
#include "../../shapes.hpp"
#include "../RegionQueryService.hpp"
#include "../debug_helper.hpp"

#include <QGuiApplication>
#include <QPaintDevice>
#include <QPainter>
#include <QPicture>
#include <QPixmap>
#include <QPoint>
#include <QRect>

#include <cassert>
#include <iostream>
#include <map>
#include <type_traits>
#include <vector>

std::map<int, IShapePtr> index2shape;
int shapes_num = 0;

template <typename T> IShapePtr create(QPoint p1, QPoint p2)
{
    IShapePtr shape = std::shared_ptr<T>(new T);
    // std::cout << shapes_num << " " << shape << std::endl;
    index2shape[++shapes_num] = shape;
    shape->addPoint(p2);
    shape->addPoint(p1);
    // if ( std::is_same<T, polygon>)
    //    shape->addPoint(p3);

    return shape;
}

bool test1()
{
    RegionQuery &rq = RegionQuery::getInstance();

    // creating data
    rq.insertObject(create<Rectangle>(QPoint(10, 10), QPoint(20, 20))); // n1
    // rq.insertObject(create<Rectangle>(QPoint(0,0),QPoint(4,5),QPoint(9,9)));     //n2
    rq.insertObject(create<Ellipse>(QPoint(8, 8), QPoint(10, 10))); // n3

    //(15,15) is in range of (10,10)-(20,20),
    // therefore Rectangle n2 should be returned
    assert(rq.getShapeUnderPos(QPoint(15, 15)) != nullptr);
    assert(rq.getShapeUnderPos(QPoint(15, 15)) != index2shape[1]);

    // region (0,0)-(100,100) should contain all the inserted objects
    assert(rq.getShapesUnderRect(QRect(0, 0, 100, 100)).size() == 3);

    // bad case, explicitly deleting shape, then adding new object to the tree
    // delete index2shape[1].get();
    // rq.insertObject(create<Ellipse>(QPoint(99,99),QPoint(105,105)));           //n4
    // assert(rq.getShapesUnderRect(QRect(0,0,100,100)).size() == 4);
}

bool test2()
{
    RegionQuery &rq = RegionQuery::getInstance();

    // creating data
    rq.insertObject(create<Rectangle>(QPoint(10, 10), QPoint(20, 20)));   // n1
    rq.insertObject(create<Rectangle>(QPoint(20, 20), QPoint(40, 40)));   // n2
    rq.insertObject(create<Rectangle>(QPoint(60, 60), QPoint(80, 80)));   // n3
    rq.insertObject(create<Rectangle>(QPoint(90, 90), QPoint(100, 100))); // n4
    rq.removeObject(index2shape[4]);
    rq.insertObject(create<Rectangle>(QPoint(0, 0), QPoint(5, 5))); // n5
    rq.removeObject(index2shape[2]);
    rq.insertObject(create<Rectangle>(QPoint(2, 2), QPoint(4, 4))); // n6
    rq.removeObject(index2shape[3]);
    rq.insertObject(index2shape[2]);
    rq.removeObject(index2shape[1]);
    rq.insertObject(create<Rectangle>(QPoint(200, 200), QPoint(400, 400))); // n7
    rq.removeObject(index2shape[7]);
    rq.insertObject(create<Rectangle>(QPoint(550, 550), QPoint(600, 600))); // n8
    rq.removeObject(index2shape[7]);
    rq.removeObject(index2shape[3]);
    rq.insertObject(index2shape[2]);
    rq.removeObject(index2shape[1]);
    delete index2shape[1].get();
    delete index2shape[2].get();
    delete index2shape[3].get();
    rq.removeObject(index2shape[3]);
    rq.insertObject(index2shape[2]);
    rq.removeObject(index2shape[1]);
    rq.insertObject(create<Rectangle>(QPoint(10, 10), QPoint(20, 20)));   // n1
    rq.insertObject(create<Rectangle>(QPoint(20, 20), QPoint(40, 40)));   // n2
    rq.insertObject(create<Rectangle>(QPoint(60, 60), QPoint(80, 80)));   // n3
    rq.insertObject(create<Rectangle>(QPoint(90, 90), QPoint(100, 100))); // n4
    rq.removeObject(index2shape[4]);
    rq.insertObject(create<Rectangle>(QPoint(0, 0), QPoint(5, 5))); // n5
    rq.removeObject(index2shape[2]);
    rq.insertObject(create<Rectangle>(QPoint(2, 2), QPoint(4, 4))); // n6
    rq.removeObject(index2shape[3]);
    rq.insertObject(index2shape[2]);
    rq.removeObject(index2shape[1]);
    rq.insertObject(create<Rectangle>(QPoint(200, 200), QPoint(400, 400))); // n7
    rq.removeObject(index2shape[7]);
    rq.insertObject(create<Rectangle>(QPoint(550, 550), QPoint(600, 600))); // n8
    rq.removeObject(index2shape[7]);
    rq.removeObject(index2shape[3]);
    rq.insertObject(index2shape[2]);
    rq.removeObject(index2shape[1]);
    rq.removeObject(index2shape[3]);
    rq.insertObject(index2shape[2]);
    rq.removeObject(index2shape[1]);

    QPixmap pic;
    pic.load("myImage.png");
    QPainter *painter = new QPainter();

    painter->begin(&pic);
    assert(rq.getShapesUnderRect(QRect(0, 0, 10000, 10000)).size() != 0);
    for (auto shape : rq.getShapesUnderRect(QRect(0, 0, 10000, 10000)))
        shape->getPoints();
    ;
    // pic.save("test.bmp");
    // pic.save("test.bmp");
    painter->end();
}

void test4()
{

    RegionQuery &rq = RegionQuery::getInstance();
    rq.clear();

    // dzumaaaaa...!!!
    rq.insertObject(create<Rectangle>(QPoint(179, 137), QPoint(83, 62)));    // 1, RQ!
    rq.insertObject(create<Rectangle>(QPoint(349, 307), QPoint(143, 202)));  // 2, RQ!
    rq.insertObject(create<Rectangle>(QPoint(563, 274), QPoint(454, 198)));  // 3, RQ!
    rq.insertObject(create<Rectangle>(QPoint(789, 250), QPoint(736, 168)));  // 4, RQ!
    rq.insertObject(create<Rectangle>(QPoint(742, 346), QPoint(620, 259)));  // 5, RQ!
    rq.insertObject(create<Rectangle>(QPoint(1028, 255), QPoint(946, 188))); // 6, RQ!
    rq.removeObject(index2shape[6]);                                         // RQ!
    rq.insertObject(create<Rectangle>(QPoint(828, 98), QPoint(746, 31)));    // 7, RQ!
    rq.removeObject(index2shape[7]);                                         // RQ!
    rq.insertObject(create<Rectangle>(QPoint(980, 262), QPoint(858, 175)));  // 8, RQ!
    rq.removeObject(index2shape[8]);                                         // RQ!
    rq.insertObject(create<Rectangle>(QPoint(503, 400), QPoint(450, 318)));  // 9, RQ!
    rq.removeObject(index2shape[9]);                                         // RQ!
    rq.insertObject(create<Rectangle>(QPoint(886, 272), QPoint(777, 196)));  // 10, RQ!
    rq.removeObject(index2shape[10]);                                        // RQ!
    rq.insertObject(create<Rectangle>(QPoint(764, 252), QPoint(558, 147)));  // 11, RQ!
    rq.removeObject(index2shape[11]);                                        // RQ!
    rq.insertObject(create<Rectangle>(QPoint(582, 201), QPoint(486, 126)));  // 12, RQ!
    rq.removeObject(index2shape[12]);                                        // RQ!
    rq.insertObject(create<Rectangle>(QPoint(179, 232), QPoint(126, 150)));  // 13, RQ!

    QPicture pic;
    pic.load("test4.pnt");
    QPainter *painter = new QPainter();

    painter->begin(&pic);
    std::vector<IShapePtr> shapes = rq.getShapesUnderRect(QRect(0, 0, 10000, 10000));
    assert(shapes.size() != 0);
    for (auto shape : shapes)
        shape->draw(painter);
    painter->end();
}

void test3()
{
    QPainter *painter = new QPainter();
    QPicture *device = new QPicture();

    RegionQuery &rq = RegionQuery::getInstance();

    rq.insertObject(create<Rectangle>(QPoint(213, 163), QPoint(111, 81)));  // 1, RQ!
    rq.insertObject(create<Rectangle>(QPoint(321, 302), QPoint(239, 226))); // 2, RQ!
    rq.insertObject(create<Rectangle>(QPoint(591, 168), QPoint(489, 86)));  // 3, RQ!
    rq.insertObject(create<Rectangle>(QPoint(679, 301), QPoint(597, 225))); // 4, RQ!

    rq.removeObject(index2shape[4]);                                       // RQ!
    rq.insertObject(create<Rectangle>(QPoint(347, 165), QPoint(245, 83))); // 5, RQ!

    rq.removeObject(index2shape[5]);                                        // RQ!
    rq.insertObject(create<Rectangle>(QPoint(420, 291), QPoint(338, 215))); // 6, RQ!

    rq.removeObject(index2shape[6]);                                        // RQ!
    rq.insertObject(create<Rectangle>(QPoint(844, 331), QPoint(762, 255))); // 7, RQ!

    rq.removeObject(index2shape[7]);                                       // RQ!
    rq.insertObject(create<Rectangle>(QPoint(778, 159), QPoint(676, 77))); // 8, RQ!

    rq.removeObject(index2shape[8]);                                        // RQ!
    rq.insertObject(create<Rectangle>(QPoint(599, 457), QPoint(497, 375))); // 9, RQ!

    rq.removeObject(index2shape[9]);                                        // RQ!
    rq.insertObject(create<Rectangle>(QPoint(196, 182), QPoint(114, 106))); // 10, RQ!

    rq.removeObject(index2shape[10]);                                       // RQ!
    rq.insertObject(create<Rectangle>(QPoint(490, 247), QPoint(408, 171))); // 11, RQ!

    rq.removeObject(index2shape[11]);                                      // RQ!
    rq.insertObject(create<Rectangle>(QPoint(348, 169), QPoint(246, 87))); // 12, RQ!

    rq.removeObject(index2shape[12]);                                       // RQ!
    rq.insertObject(create<Rectangle>(QPoint(889, 308), QPoint(787, 226))); // 13, RQ!

    rq.removeObject(index2shape[13]);                                       // RQ!
    rq.insertObject(create<Rectangle>(QPoint(916, 211), QPoint(834, 135))); // 14, RQ!

    rq.insertObject(create<Rectangle>(QPoint(335, 292), QPoint(214, 216))); // 15, RQ!
    rq.insertObject(create<Rectangle>(QPoint(572, 308), QPoint(297, 248))); // 16, RQ!

    rq.removeObject(index2shape[16]);                                        // RQ!
    rq.insertObject(create<Rectangle>(QPoint(1050, 263), QPoint(775, 203))); // 17, RQ!

    rq.removeObject(index2shape[17]);                                       // RQ!
    rq.insertObject(create<Rectangle>(QPoint(721, 343), QPoint(619, 261))); // 18, RQ!

    rq.removeObject(index2shape[18]);                                       // RQ!
    rq.insertObject(create<Rectangle>(QPoint(975, 251), QPoint(854, 175))); // 19, RQ!

    painter->begin(device);
    std::vector<IShapePtr> shapes = rq.getShapesUnderRect(QRect(0, 0, 500, 500));
    std::cout << "shapes " << shapes.size() << std::endl;
    for (auto shape : shapes)
    {
        shape->draw(painter);
        // std::cout << get_points_str(shape) << std::endl;
    }
    painter->end();

    device->save("testing.pic");
}

void test5()
{
    QPainter *painter = new QPainter();
    QPicture *device = new QPicture();

    RegionQuery &rq = RegionQuery::getInstance();
    rq.clear();
    index2shape.clear();
    shapes_num = 0;

    rq.insertObject(create<Ellipse>(QPoint(192, 140), QPoint(96, 58)));    // 1, RQ!
    rq.insertObject(create<Ellipse>(QPoint(298, 263), QPoint(183, 173)));  // 2, RQ!
    rq.removeObject(index2shape[2]);                                       // RQ!
    rq.insertObject(create<Ellipse>(QPoint(795, 249), QPoint(680, 159)));  // 3, RQ!
    rq.removeObject(index2shape[3]);                                       // RQ!
    rq.insertObject(create<Ellipse>(QPoint(335, 321), QPoint(220, 231)));  // 4, RQ!
    rq.removeObject(index2shape[1]);                                       // RQ!
    rq.insertObject(create<Ellipse>(QPoint(732, 247), QPoint(636, 165)));  // 5, RQ!
    rq.removeObject(index2shape[4]);                                       // RQ!
    rq.insertObject(create<Ellipse>(QPoint(711, 159), QPoint(596, 69)));   // 6, RQ!
    rq.removeObject(index2shape[5]);                                       // RQ!
    rq.insertObject(create<Ellipse>(QPoint(340, 269), QPoint(244, 187)));  // 7, RQ!
    rq.removeObject(index2shape[6]);                                       // RQ!
    rq.insertObject(create<Ellipse>(QPoint(946, 310), QPoint(831, 220)));  // 8, RQ!
    rq.insertObject(create<Ellipse>(QPoint(521, 144), QPoint(405, 73)));   // 9, RQ!
    rq.removeObject(index2shape[9]);                                       // RQ!
    rq.insertObject(create<Ellipse>(QPoint(729, 178), QPoint(613, 107)));  // 10, RQ!
    rq.removeObject(index2shape[7]);                                       // RQ!
    rq.insertObject(create<Ellipse>(QPoint(228, 219), QPoint(132, 137)));  // 11, RQ!
    rq.removeObject(index2shape[10]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(447, 353), QPoint(331, 282)));  // 12, RQ!
    rq.removeObject(index2shape[8]);                                       // RQ!
    rq.insertObject(create<Ellipse>(QPoint(488, 164), QPoint(373, 74)));   // 13, RQ!
    rq.removeObject(index2shape[12]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(763, 169), QPoint(647, 98)));   // 14, RQ!
    rq.removeObject(index2shape[13]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(202, 248), QPoint(87, 158)));   // 15, RQ!
    rq.removeObject(index2shape[14]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(1011, 267), QPoint(895, 196))); // 16, RQ!
    rq.removeObject(index2shape[15]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(750, 128), QPoint(635, 38)));   // 17, RQ!
    rq.removeObject(index2shape[16]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(345, 270), QPoint(229, 199)));  // 18, RQ!
    rq.removeObject(index2shape[17]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(830, 354), QPoint(715, 264)));  // 19, RQ!
    rq.removeObject(index2shape[18]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(212, 165), QPoint(96, 94)));    // 20, RQ!
    rq.removeObject(index2shape[19]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(456, 167), QPoint(341, 77)));   // 21, RQ!
    rq.removeObject(index2shape[21]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(683, 167), QPoint(568, 77)));   // 22, RQ!
    rq.removeObject(index2shape[22]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(430, 123), QPoint(315, 33)));   // 23, RQ!
    rq.insertObject(create<Ellipse>(QPoint(387, 218), QPoint(279, 131)));  // 24, RQ!
    rq.insertObject(create<Ellipse>(QPoint(536, 291), QPoint(388, 241)));  // 25, RQ!
    rq.insertObject(create<Ellipse>(QPoint(737, 287), QPoint(574, 209)));  // 26, RQ!
    rq.removeObject(index2shape[26]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(870, 174), QPoint(707, 96)));   // 27, RQ!
    rq.removeObject(index2shape[23]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(809, 274), QPoint(694, 184)));  // 28, RQ!
    rq.removeObject(index2shape[24]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(632, 204), QPoint(524, 117)));  // 29, RQ!
    rq.removeObject(index2shape[29]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(325, 169), QPoint(217, 82)));   // 30, RQ!
    rq.removeObject(index2shape[20]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(787, 278), QPoint(671, 207)));  // 31, RQ!
    rq.removeObject(index2shape[28]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(165, 169), QPoint(50, 79)));    // 32, RQ!
    rq.removeObject(index2shape[27]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(1061, 215), QPoint(898, 137))); // 33, RQ!
    rq.removeObject(index2shape[30]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(624, 156), QPoint(516, 69)));   // 34, RQ!
    rq.removeObject(index2shape[34]);                                      // RQ!
    rq.insertObject(create<Ellipse>(QPoint(429, 201), QPoint(321, 114)));  // 35, RQ!

    painter->begin(device);
    std::vector<IShapePtr> shapes = rq.getShapesUnderRect(QRect(0, 0, 1200, 381));
    std::cout << "shapes " << shapes.size() << std::endl;
    for (auto shape : shapes)
    {
        shape->draw(painter);
        // std::cout << get_lvi_format(shape) << std::endl;
    }
    painter->end();

    device->save("testing.pic");
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    // main_window window;
    // test1();
    // test2();
    // test3();
    test5();
}
