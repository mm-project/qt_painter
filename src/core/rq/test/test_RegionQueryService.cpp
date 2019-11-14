#include "../RegionQueryService.hpp"
#include "../debug_helper.hpp"
#include "../../ishape.hpp"
#include "../../shapes.hpp"


#include <QPoint>
#include <QRect>
#include <QPainter>
#include <QPixmap>
#include <QPicture>
#include <QPaintDevice>
#include <QGuiApplication>

#include <cassert>
#include <map>
#include <vector>
#include <type_traits>

std::map<int,IShape*> index2shape;
int shapes_num;

template<typename T>
IShape* create(QPoint p1,  QPoint p2, QPoint p3 = QPoint())
{
    IShape* shape = new T;
    index2shape[++shapes_num] = shape;
    shape->addPoint(p2);
    shape->addPoint(p1);
    //if ( std::is_same<T, polygon>)
    //   shape->addPoint(p3);
        
    return shape;    
}


bool test1() 
{
    RegionQuery& rq = RegionQuery::getInstance();

    //creating data
    rq.insertObject(create<rectangle>(QPoint(10,10),QPoint(20,20)));           //n1
    rq.insertObject(create<rectangle>(QPoint(0,0),QPoint(4,5),QPoint(9,9)));     //n2
    rq.insertObject(create<ellipse>(QPoint(8,8),QPoint(10,10)));               //n3
    
    //(15,15) is in range of (10,10)-(20,20), 
    //therefore rectangle n2 should be returned
    assert(rq.getShapeUnderPos(QPoint(15,15)) != nullptr);
    assert(rq.getShapeUnderPos(QPoint(15,15)) != index2shape[1]);
    
    //region (0,0)-(100,100) should contain all the inserted objects
    assert(rq.getShapesUnderRect(QRect(0,0,100,100)).size() == 3);
    
    //bad case, explicitly deleting shape, then adding new object to the tree
    //delete index2shape[1];
    //rq.insertObject(create<ellipse>(QPoint(99,99),QPoint(105,105)));           //n4
    //assert(rq.getShapesUnderRect(QRect(0,0,100,100)).size() == 4);
}


bool test2()
{
    RegionQuery& rq = RegionQuery::getInstance();

    //creating data
    rq.insertObject(create<rectangle>(QPoint(10,10),QPoint(20,20)));             //n1
    rq.insertObject(create<rectangle>(QPoint(20,20),QPoint(40,40)));             //n2
    rq.insertObject(create<rectangle>(QPoint(60,60),QPoint(80,80)));             //n3
    rq.insertObject(create<rectangle>(QPoint(90,90),QPoint(100,100)));           //n4
    rq.removeObject(index2shape[4]);
    rq.insertObject(create<rectangle>(QPoint(0,0),QPoint(5,5)));                 //n5
    rq.removeObject(index2shape[2]);
    rq.insertObject(create<rectangle>(QPoint(2,2),QPoint(4,4)));                 //n6
    rq.removeObject(index2shape[3]);
    rq.insertObject(index2shape[2]);
    rq.removeObject(index2shape[1]);
    rq.insertObject(create<rectangle>(QPoint(200,200),QPoint(400,400)));         //n7
    rq.removeObject(index2shape[7]);
    rq.insertObject(create<rectangle>(QPoint(550,550),QPoint(600,600)));         //n8
    rq.removeObject(index2shape[7]);
    rq.removeObject(index2shape[3]);
    rq.insertObject(index2shape[2]);
    rq.removeObject(index2shape[1]);
    delete index2shape[1];
    delete index2shape[2];
    delete index2shape[3];
    rq.removeObject(index2shape[3]);
    rq.insertObject(index2shape[2]);
    rq.removeObject(index2shape[1]);
    rq.insertObject(create<rectangle>(QPoint(10,10),QPoint(20,20)));             //n1
    rq.insertObject(create<rectangle>(QPoint(20,20),QPoint(40,40)));             //n2
    rq.insertObject(create<rectangle>(QPoint(60,60),QPoint(80,80)));             //n3
    rq.insertObject(create<rectangle>(QPoint(90,90),QPoint(100,100)));           //n4
    rq.removeObject(index2shape[4]);
    rq.insertObject(create<rectangle>(QPoint(0,0),QPoint(5,5)));                 //n5
    rq.removeObject(index2shape[2]);
    rq.insertObject(create<rectangle>(QPoint(2,2),QPoint(4,4)));                 //n6
    rq.removeObject(index2shape[3]);
    rq.insertObject(index2shape[2]);
    rq.removeObject(index2shape[1]);
    rq.insertObject(create<rectangle>(QPoint(200,200),QPoint(400,400)));         //n7
    rq.removeObject(index2shape[7]);
    rq.insertObject(create<rectangle>(QPoint(550,550),QPoint(600,600)));         //n8
    rq.removeObject(index2shape[7]);
    rq.removeObject(index2shape[3]);
    rq.insertObject(index2shape[2]);
    rq.removeObject(index2shape[1]);
    rq.removeObject(index2shape[3]);
    rq.insertObject(index2shape[2]);
    rq.removeObject(index2shape[1]);

    QPixmap pic;
    pic.load("myImage.png");
    QPainter* painter = new QPainter();

    painter->begin(&pic);
    assert(rq.getShapesUnderRect(QRect(0,0,10000,10000)).size() != 0);
    for ( auto shape : rq.getShapesUnderRect(QRect(0,0,10000,10000)) )
        shape->getPoints();;
    //pic.save("test.bmp");
    //pic.save("test.bmp");
    painter->end();    
}



void test4() 
{
    
    RegionQuery& rq = RegionQuery::getInstance();
    rq.clean();
      
    //dzumaaaaa...!!!
    rq.insertObject(create<rectangle>(QPoint(179,137),QPoint(83,62)));  //1, RQ!
    rq.insertObject(create<rectangle>(QPoint(349,307),QPoint(143,202)));  //2, RQ!
    rq.insertObject(create<rectangle>(QPoint(563,274),QPoint(454,198)));  //3, RQ!
    rq.insertObject(create<rectangle>(QPoint(789,250),QPoint(736,168)));  //4, RQ!
    rq.insertObject(create<rectangle>(QPoint(742,346),QPoint(620,259)));  //5, RQ!
    rq.insertObject(create<rectangle>(QPoint(1028,255),QPoint(946,188)));  //6, RQ!
    rq.removeObject(index2shape[6]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(828,98),QPoint(746,31)));  //7, RQ!
    rq.removeObject(index2shape[7]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(980,262),QPoint(858,175)));  //8, RQ!
    rq.removeObject(index2shape[8]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(503,400),QPoint(450,318)));  //9, RQ!
    rq.removeObject(index2shape[9]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(886,272),QPoint(777,196)));  //10, RQ!
    rq.removeObject(index2shape[10]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(764,252),QPoint(558,147)));  //11, RQ!
    rq.removeObject(index2shape[11]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(582,201),QPoint(486,126)));  //12, RQ!
    rq.removeObject(index2shape[12]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(179,232),QPoint(126,150)));  //13, RQ!
    
    
    QPicture pic;
    pic.load("test4.pnt");
    QPainter* painter = new QPainter();

    painter->begin(&pic);
    std::vector<IShape*> shapes = rq.getShapesUnderRect(QRect(0,0,10000,10000));
    assert(shapes.size() != 0);
    for ( auto shape : shapes )
        shape->draw(painter);
    painter->end();    
    
}



void test3()
{
    QPainter* painter = new QPainter();
    QPicture* device = new QPicture();
    
    RegionQuery& rq = RegionQuery::getInstance();

    rq.insertObject(create<rectangle>(QPoint(213,163),QPoint(111,81)));  //1, RQ!
    rq.insertObject(create<rectangle>(QPoint(321,302),QPoint(239,226)));  //2, RQ!
    rq.insertObject(create<rectangle>(QPoint(591,168),QPoint(489,86)));  //3, RQ!
    rq.insertObject(create<rectangle>(QPoint(679,301),QPoint(597,225)));  //4, RQ!

    rq.removeObject(index2shape[4]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(347,165),QPoint(245,83)));  //5, RQ!

    rq.removeObject(index2shape[5]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(420,291),QPoint(338,215)));  //6, RQ!

    rq.removeObject(index2shape[6]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(844,331),QPoint(762,255)));  //7, RQ!

    rq.removeObject(index2shape[7]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(778,159),QPoint(676,77)));  //8, RQ!

    rq.removeObject(index2shape[8]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(599,457),QPoint(497,375)));  //9, RQ!

    rq.removeObject(index2shape[9]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(196,182),QPoint(114,106)));  //10, RQ!

    rq.removeObject(index2shape[10]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(490,247),QPoint(408,171)));  //11, RQ!

    rq.removeObject(index2shape[11]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(348,169),QPoint(246,87)));  //12, RQ!

    rq.removeObject(index2shape[12]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(889,308),QPoint(787,226)));  //13, RQ!

    rq.removeObject(index2shape[13]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(916,211),QPoint(834,135)));  //14, RQ!

    rq.insertObject(create<rectangle>(QPoint(335,292),QPoint(214,216)));  //15, RQ!
    rq.insertObject(create<rectangle>(QPoint(572,308),QPoint(297,248)));  //16, RQ!

    rq.removeObject(index2shape[16]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(1050,263),QPoint(775,203)));  //17, RQ!

    rq.removeObject(index2shape[17]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(721,343),QPoint(619,261)));  //18, RQ!

    rq.removeObject(index2shape[18]);         //RQ!
    rq.insertObject(create<rectangle>(QPoint(975,251),QPoint(854,175)));  //19, RQ!

    painter->begin(device);
    std::vector<IShape*> shapes = rq.getShapesUnderRect(QRect(0,0,500,500));
    std::cout << "shapes " << shapes.size() << std::endl;
    for ( auto shape : shapes ) {
        shape->draw(painter);
        std::cout << get_points_str(shape) << std::endl;    
    }
    painter->end();
 
    device->save("testing.pic");
    
}

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);
    //main_window window;
    //test1();
    //test2();
    //test3();
    test4();
}
