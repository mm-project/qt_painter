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

int canvas_w = 1200;
int canvas_h = 400;
int num_threads = 16;
int num_regions = num_threads;

int query_w;
int query_h;

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
    for(int column=0; column<40; column++)
        for(int row=0; row<40; row++)
            //create_rect(0,0,100,100);
            create_rect_at_given_cell_and_row(column,row);
    /**/

}

QPixmap put_shapes_on_pixmap(auto shapes, int i, int w, int h)
{
    QPixmap pixmap(w,h);
    QColor color(255/(1+i), 255/(1+i), 255/(1+i));
    pixmap.fill(color);

    QPainter *painter = new QPainter(&pixmap);
    for (const auto& shape : shapes)
        shape->draw(painter);

    pixmap.save("region" + QString::number(i) + ".bmp");

    delete painter;
    return pixmap;

}


std::vector<QRect> init_query_rects(int num_regions)
{

    std::vector<QRect> query_rects(num_regions);

    int factor;
    switch(num_regions) {
        case 4:
            factor = 2;
            break;
        case 16:
            factor = 4;
            break;
        case 64:
            factor = 8;
            break;
    }

    query_w = canvas_w/factor;
    query_h = canvas_h/factor;

    int k=0;
    for(int i=0;i<factor;i++)
        for(int j=0;j<factor;j++) {
            std::cout << "creating region " << k << std::endl;
            query_rects[k] = QRect(j*canvas_w/factor,i*canvas_h/factor,query_w,query_h);
            k++;
        }

    return query_rects;
}


bool test_mt_rendering()
{
    RegionQuery &rq = RegionQuery::getInstance();

    std::vector<QRect> query_rects = init_query_rects(num_regions);
    std::vector<QPixmap> pixmaps(num_regions);

    #pragma omp parallel for
    for(int i=0;i<num_regions;i++){
        auto shapes = rq.getShapesUnderRect(query_rects[i]);
        std::cout << "region" << i << ":   shapes:" << shapes.size() << std::endl;
        pixmaps[i] = put_shapes_on_pixmap(shapes,i,canvas_w,canvas_h);
    }

    QPixmap canvas(canvas_w,canvas_h);
    canvas.fill(Qt::black);
    QPainter *painter = new QPainter(&canvas);
    painter->setPen(QColor(255,34,255,255));
    for(int i=0;i<num_regions;i++)
        painter->drawPixmap(query_rects[i].x(),query_rects[i].y(),query_w,query_h,pixmaps[i],query_rects[i].x(),query_rects[i].y(),query_w,query_h);
    canvas.save("canvas.bmp");
    delete painter;

}

/*
bool put_all_pixmaps_to_canvas()
{

    QPixmap canvas(canvas_w,canvas_h);
    canvas.fill(Qt::black);
    QPainter *painter = new QPainter(&canvas);
    painter->setPen(QColor(255,34,255,255));


    canvas.save("canvas.bmp");
    delete painter;

}
*/


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
    test_mt_rendering();
    //put_all_pixmaps_to_canvas();

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
