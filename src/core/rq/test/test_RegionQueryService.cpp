#include "../RegionQueryService.hpp"
#include "../../ishape.hpp"
#include "../../shapes.hpp"

#include <QPoint>
#include <QRect>

#include <cassert>
#include <map>
#include <type_traits>

std::map<int,IShape*> index2shape;
int shapes_num;

template<typename T>
IShape* create(QPoint p1,  QPoint p2, QPoint p3 = QPoint())
{
    IShape* shape = new T;
    index2shape[shapes_num++] = shape;
    shape->addPoint(p1);
    shape->addPoint(p2);
    //if ( std::is_same<T, polygon>)
    //   shape->addPoint(p3);
        
    return shape;    
}


int main()
{
    RegionQuery& rq = RegionQuery::getInstance();

    //creating data
    rq.insertObject(create<rectangle>(QPoint(10,10),QPoint(20,20)));           //n1
    rq.insertObject(create<polygon>(QPoint(0,0),QPoint(4,5),QPoint(9,9)));     //n2
    rq.insertObject(create<ellipse>(QPoint(8,8),QPoint(10,10)));               //n3
    
    //(15,15) is in range of (10,10)-(20,20), 
    //therefore rectangle n2 should be returned
    assert(rq.getShapeUnderPos(QPoint(15,15)) != nullptr);
    assert(rq.getShapeUnderPos(QPoint(15,15)) != index2shape[1]);
    
    //region (0,0)-(100,100) should contain all the inserted objects
    assert(rq.getShapesUnderRect(QRect(0,0,100,100)).size() == 3);
    
    //bad case, explicitly deleting shape, then adding new object to the tree
    delete index2shape[1];
    rq.insertObject(create<ellipse>(QPoint(99,99),QPoint(105,105)));           //n4
    assert(rq.getShapesUnderRect(QRect(0,0,100,100)).size() == 4);
    
}
