#include "../RegionQueryService.hpp"
#include "../debug_helper.hpp"

#include <QPoint>
#include <QPainter>
#include <QPixmap>
#include <QGuiApplication>

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
    int delta = 80;
    int epsilon = 70;
    create_rect(delta*row,delta*column,delta*row-epsilon,delta*column-epsilon);
}


//bool test_rq_by_inserting_1k_by_1k_objs()
bool create_design()
{

    /*
    create_rect(28,25,295,140);
    create_rect(989,39,1060,179);
    create_rect(33,279,452,330);
    create_rect(877,253,1165,345);
    */

    //*
    for(int column=0; column<200; column++)
        for(int row=0; row<200; row++)
            //create_rect(0,0,100,100);
            create_rect_at_given_cell_and_row(column,row);
    /**/

}

QPixmap get_shapes_on_pixmap(auto shapes, int i, int w, int h)
{
    QPixmap pixmap(w,h);
    pixmap.fill(Qt::blue);
    QPainter *painter = new QPainter(&pixmap);
    //canvas->setPen(QColor(255,34,255,255));
    //canvas->drawRect(15,15,100,100);
    //pix.save("test.bmp")
    for (const auto& shape : shapes)
        shape->draw(painter);

    //pixmap.save("region" + QString::number(i) + ".bmp");

    delete painter;
    return pixmap;

}

/*
bool test_mt_rq()
{
    RegionQuery &rq = RegionQuery::getInstance();
    int delta = 100;
    int epsilon = 20;
    std::vector<QPixmap> regions(4);
    #pragma omp parallel for
    for(int i=0;i<4;i++){
        //create_rect(delta*row,delta*column,delta*row-epsilon,delta*column-epsilon);
        int x = delta*i;
        int y = delta*i;
        int w = epsilon;
        int h = epsilon;
        //std::cout << x << " " << y << " " << x + w << " " << y + h << std::endl;
        auto shapes = rq.getShapesUnderRect(QRect(QPoint(x,y),QPoint(x+w,y+h)));
        regions[i] = get_shapes_on_pixmap(shapes,i,x+w,y+h);
    }

    QPixmap canvas(2000,2000);
    QPainter *painter = new QPainter(&canvas);
    painter->setPen(QColor(255,34,255,255));
    painter->drawPixmap(0,0,2000,2000,regions[0],0,0,1000,1000);
    canvas.save("test.bmp");
    delete painter;

}
*/

bool test_mt_rq()
{
    RegionQuery &rq = RegionQuery::getInstance();
    int delta = 100;
    int epsilon = 20;
    int canvas_w = 1200;
    int canvas_h = 400;
    int query_w = canvas_w/2;
    int query_h = canvas_h/2;

    int mid_x = canvas_w/2;
    int mid_y = canvas_h/2;

    std::vector<QRect> query_regions(4);
    query_regions[0] = QRect(0,0,query_w,query_h);
    query_regions[1] = QRect(mid_x,0,query_w,query_h);
    query_regions[2] = QRect(0,mid_y,query_w,query_h);
    query_regions[3] = QRect(mid_x,mid_y,query_w,query_h);



    std::vector<QPixmap> regions(4);
    #pragma omp parallel for
    for(int i=0;i<4;i++){
        auto shapes = rq.getShapesUnderRect(query_regions[i]);
        std::cout << "region" << i << ":   shapes:" << shapes.size() << std::endl;
        regions[i] = get_shapes_on_pixmap(shapes,i,canvas_w,canvas_h);
    }

    QPixmap canvas(canvas_w,canvas_h);
    canvas.fill(Qt::black);
    QPainter *painter = new QPainter(&canvas);
    painter->setPen(QColor(255,34,255,255));
    painter->drawPixmap(0,0,query_w,query_h,regions[0],0,0,query_w,query_h);
    painter->drawPixmap(mid_x,0,query_w,query_h,regions[1],mid_x,0,query_w,query_h);
    painter->drawPixmap(0,mid_y,query_w,query_h,regions[2],0,mid_y,query_w,query_h);
    painter->drawPixmap(mid_x,mid_y,query_w,query_h,regions[3],mid_x,mid_y,query_w,query_h);

    canvas.save("canvas.bmp");
    delete painter;


    /*
    QPixmap canvas(canvas_w,canvas_h);
    canvas.fill(Qt::black);
    QPainter *painter = new QPainter(&canvas);
    painter->setPen(QColor(255,34,255,255));
    for (const auto& shape : rq.getShapesUnderRect(QRect(0,0,canvas_w,canvas_h)))
        shape->draw(painter);

    //painter->drawPixmap(0,0,2000,2000,regions[0],0,0,1000,1000);
    canvas.save("canvas.bmp");
    delete painter;
    */

}


int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    /*
    QPixmap canvas(2000,2000);
    QPainter *paint = new QPainter(&canvas);
    canvas->setPen(QColor(255,34,255,255));
    canvas->drawRect(15,15,100,100);
    pix.save("test.bmp")
    // delete paint;
    //scene->addPixmap(pix);
    */

    create_design();
    test_mt_rq();

    //QPixmap pic(1000,1000);//("myImage.png");
    //pic.load("myImage.png");
    //QPainter *painter = new QPainter(&pic);
    //painter->begin(&pic);
    //painter->drawRect(0,0,100,100);
    //painter->end();

    //painter->begin();
    /*
    RegionQuery &rq = RegionQuery::getInstance();
    assert(rq.getShapesUnderRect(QRect(0, 0, 10000, 10000)).size() != 0);
    for (auto shape : rq.getShapesUnderRect(QRect(0, 0, 10000, 10000)))
        shape->getPoints();
    //painter->end();
    //pic.save("test.bmp");
    */
    //test_mt_rq();
    ;
    // pic.save("test.bmp");

    std::cout << "pass" << std::endl;
}
