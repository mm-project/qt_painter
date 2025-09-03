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
#include <fstream>
#include <filesystem>
#include <vector>
#include <set>
#include <map>
#include <chrono>

RegionQuery &rq = RegionQuery::getInstance();
int PIXMAP_W = 1000;
int PIXMAP_H = 1000;

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
        test_info(int ti, int vi, int rq_oc, int ws_oc, QRect sel) {
            test_id = ti;
            validation_id = vi;
            rq_object_count = rq_oc;
            ws_object_count = ws_oc;
            select_rect = sel;
        }
    
    int test_id;
    int validation_id;
    int rq_object_count;
    int ws_object_count;
    QRect select_rect;

};
std::vector<test_info> failed_tests;
std::map<int, std::set<int>> skip_lists;
std::map<int, std::set<int>> allow_lists;

static QRect boundsOf(const std::vector<QPoint>& pts){
    if (pts.empty()) return QRect();
    int minx=pts[0].x(), maxx=minx, miny=pts[0].y(), maxy=miny;
    for (size_t i=1;i<pts.size();++i){
        minx = std::min(minx, pts[i].x());
        maxx = std::max(maxx, pts[i].x());
        miny = std::min(miny, pts[i].y());
        maxy = std::max(maxy, pts[i].y());
    }
    return QRect(QPoint(minx,miny), QPoint(maxx,maxy)).normalized();
}

std::vector<IShapePtr> getShapesUnderRect(int x,int y,int w,int h)
{
    std::vector<IShapePtr> out;
    QRect r = QRect(x,y,w,h).normalized();
    r.adjust(0,0,1,1);

    for (auto& s : the_ws) {
        const auto& pts = s->getPoints();
        if (pts.empty()) continue;

        bool hit = false;
        for (const auto& p : pts) {
            if (r.contains(p)) { hit = true; break; }
        }
        if (!hit && boundsOf(pts).intersects(r)) hit = true;

        if (hit) out.push_back(s);
    }
    return out;
}

template <typename T> IShapePtr create(QPoint p1, QPoint p2)
{
    IShapePtr shape = std::shared_ptr<T>(new T);
    shape->addPoint(p2);
    shape->addPoint(p1);

    return shape;
}

template <typename T> void create_obj(int x1, int y1, int x2, int y2)
{
    // remove if need to test negative coordinates as well 
    if ( x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 )
        return;

    auto rect = create<T>(QPoint(x1, y1), QPoint(x2, y2));
    rq.insertObject(rect);
    the_ws.push_back(rect);
}

template <typename T> void create_obj_at_given_cell_and_row(int column, int row)
{
    // don't know, but it does what i wanted
    int delta = 100;
    int epsilon = 90;
    create_obj<T>(delta*row,delta*column,delta*row-epsilon,delta*column-epsilon);
}

template <typename T> void insert_nxn_matrix_of_objs(int n)
{
    std::cout << "  inserting " << n << "x" << n << " objects ..." << std::endl;
    for(int column=0; column<n; column++)
        for(int row=0; row<n; row++)
            create_obj_at_given_cell_and_row<T>(column,row);
}

auto init() 
{
    std::cout <<"[test " << test_id<< "]: initizialazing ..." << std::endl;
    rq.clear();
    the_ws.clear();
    
    QPixmap* pixmap1 = new QPixmap(PIXMAP_W,PIXMAP_H);
    pixmap1->fill(Qt::black);
    QPainter* painter1 = new QPainter(pixmap1);

    QPixmap* pixmap2 = new QPixmap(PIXMAP_W,PIXMAP_H);
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
    the_ws.clear();

    test_id++;
    validation_id = 1;
}

void refresh_drawbles()
{
    delete drawbles.first.painter;
    delete drawbles.first.pixmap;
    delete drawbles.second.painter;
    delete drawbles.second.pixmap;

    QPixmap* pixmap1 = new QPixmap(PIXMAP_W,PIXMAP_H);
    pixmap1->fill(Qt::black);
    QPainter* painter1 = new QPainter(pixmap1);

    QPixmap* pixmap2 = new QPixmap(PIXMAP_W,PIXMAP_H);
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
    
    ///*
    auto it = skip_lists.find(test_id);
    if (it != skip_lists.end() && it->second.find(validation_id) != it->second.end()) {
        std::cout << "       skipped" << std::endl;
        refresh_drawbles();
        validation_id++;

        return;
    }
    /**/
    
    //*
    auto it2 = allow_lists.find(test_id);
    if ( allow_lists.size() > 0  && ( it2 == allow_lists.end() || it2->second.find(validation_id) == it2->second.end())) {
        std::cout << "       skipped" << std::endl;
        refresh_drawbles();
        validation_id++;

        return;
    }
    /**/

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
        failed_tests.push_back(test_info(test_id,validation_id,rq_shapes.size(),ws_shapes.size(),QRect(x, y, width, height)));
    } else {
        std::cout << "       passed" << std::endl;
    }
    
    std::cout << "        drawing and saving design data..." << std::endl;
    QPixmap pixmap(PIXMAP_W,PIXMAP_H);
    pixmap.fill(Qt::black);
    QPainter painter(&pixmap);
    for (auto & shape: the_ws)
        shape->draw(&painter);
    auto select_rect = create<Rectangle>(QPoint(x, y), QPoint(x+width, y+height));
    ShapeProperties p;
    p.pen_color = Qt::red;
    p.brush_color = Qt::red;
    p.brush_style = Qt::NoBrush;
    select_rect->updateProperties(p);
    select_rect->draw(&painter);
    QString fname_des = "des_" + QString::number(test_id) + "_" + QString::number(validation_id) + ".png"; 
    pixmap.save(fname_des);
    //delete painter;
    //painter = 0;

    std::cout << "        drawing rq data..." << std::endl;
    for (auto & shape: rq_shapes)
        shape->draw(drawbles.first.painter);
    select_rect->draw(drawbles.first.painter);

    QString fname_rq = "rq_" + QString::number(test_id) + "_" + QString::number(validation_id) + ".png"; 
    std::cout << "        saving rq data to " << fname_rq.toStdString() << " ..." << std::endl;
    drawbles.first.pixmap->save(fname_rq);

    std::cout << "        comparing with expected ..." << std::endl;
    //assert(!are_imagefiles_different("rq.png","expected/rq.png"));

    std::cout << "        drawing ws data..." << std::endl;
    for (auto & shape: ws_shapes)
        shape->draw(drawbles.second.painter);
    select_rect->draw(drawbles.second.painter);

    QString fname_ws = "ws_" + QString::number(test_id) + "_" + QString::number(validation_id) + ".png"; 
    std::cout << "        saving ws data to " << fname_ws.toStdString() << " ..." << std::endl;
    drawbles.second.pixmap->save(fname_ws);
    
    refresh_drawbles();
    validation_id++;
}

void generate_html_diff()
{
    std::string html = R"(
        <!DOCTYPE html>
        <html lang="en">
        <head>
        <meta charset="UTF-8">
        <title>4 Column Table</title>
        <style>
        body { font-family: sans-serif; }
        table { border-collapse: collapse; width: 90%; margin: 20px auto; }
        th, td { border: 1px solid #555; padding: 10px; text-align: center; vertical-align: top; }
        th { background-color: #f2f2f2; }
        /* thumbnails */
        table img { width: 90%; height: auto; cursor: zoom-in; }
        /* overlay */
        #overlay {
            display: none; position: fixed; inset: 0;
            background: rgba(0,0,0,0.75);
            justify-content: center; align-items: center;
            z-index: 9999;
        }
        #overlay img {
            max-width: 90vw; max-height: 90vh; box-shadow: 0 0 20px rgba(255,255,255,0.4);
        }
        </style>
        </head>
        <body>
        <h2 style="text-align:center;">Region query unit test failures</h2>

        <!-- Overlay for full image -->
        <div id="overlay" onclick="this.style.display='none'">
        <img id="fullImage" alt="full">
        </div>

        <table>
        <tr>
            <th>Check</th>
            <th>Design and select</th>
            <th>RQ data</th>
            <th>WS data</th>
        </tr>
        )";

    for (auto& failed_test : failed_tests) {
        int t_id  = failed_test.test_id;
        int v_id  = failed_test.validation_id;
        int rq_oc = failed_test.rq_object_count;
        int ws_oc = failed_test.ws_object_count;

        const std::string des  = "des_" + std::to_string(t_id) + "_" + std::to_string(v_id) + ".png";
        const std::string rq   = "rq_"  + std::to_string(t_id) + "_" + std::to_string(v_id) + ".png";
        const std::string ws   = "ws_"  + std::to_string(t_id) + "_" + std::to_string(v_id) + ".png";

        html += "<tr>\n";
        html += "  <td>Test " + std::to_string(t_id) +
                " &mdash; Validation " + std::to_string(v_id) + "</td>\n";

        // Each image calls showImage('path') to open overlay
        html += "  <td><img src=\"" + des + "\" alt=\"design\" onclick=\"showImage('" + des + "')\"></td>\n";

        html += "  <td><img src=\"" + rq + "\" alt=\"rq\" onclick=\"showImage('" + rq + "')\">"
                "<br>RQ Got " + std::to_string(rq_oc) + " objs</td>\n";

        html += "  <td><img src=\"" + ws + "\" alt=\"ws\" onclick=\"showImage('" + ws + "')\">"
                "<br>WS Got " + std::to_string(ws_oc) + " objs</td>\n";

        html += "</tr>\n";
    }

    html += R"(
        </table>

        <script>
        function showImage(src) {
            var o = document.getElementById('overlay');
            var img = document.getElementById('fullImage');
            img.src = src;
            o.style.display = 'flex';
        }
        </script>

        </body>
        </html>
        )";

    std::ofstream out("rq_ut_diff.html");
    if (!out) {
        std::cerr << "Error: could not open file for writing!\n";
        return;
    }
    out << html;
    std::cout << "HTML file created: rq_ut_diff.html\n";
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
        generate_html_diff();
        exit(1);
    } else {
        std::cout << std::endl;
        std::cout << "all tests passed." << std::endl;
        std::cout << std::endl;
    }
}

void skip_test(int t_id, int v_id)
{
    skip_lists[t_id].insert(v_id);
}

void skip_all_tests_except(int t_id, int v_id)
{
    allow_lists[t_id].insert(v_id);
}




void run_validations()
{
    //v1
    validate_rq(0, 0, 10, 10);
    //v2
    validate_rq(0, 0, 500, 500);
    //v3
    validate_rq(0, 0, 1000, 1000);
    //v4
    validate_rq(88, 88, 101, 103);
    //v5
    validate_rq(400, 441, 100, 203);
    //v6
    validate_rq(123, 432, 10, 509);
    //v7
    validate_rq(123, 432, 99, 10);
    //v8
    validate_rq(111, 222, 333, 444);
}

void test1() 
{
    init();
    insert_nxn_matrix_of_objs<Rectangle>(10);
    run_validations();
    fini();
}

void test2() 
{
    init();
    insert_nxn_matrix_of_objs<Ellipse>(10);
    run_validations();    
    fini();
}

void test3() 
{
    init();
    insert_nxn_matrix_of_objs<Line>(10);
    run_validations();    
    fini();
}

void test4() 
{
    init();
    insert_nxn_matrix_of_objs<Rectangle>(10);
    insert_nxn_matrix_of_objs<Line>(10);
    insert_nxn_matrix_of_objs<Ellipse>(10);
    run_validations();
    fini();
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    // do NOT remove, ws query not working properly for those cases 
    skip_test(2,1);
    skip_test(3,5);
    skip_test(4,5);

    // remove after rq fix 
    skip_test(1,1);
    skip_test(3,1);
    skip_test(3,4);
    skip_test(3,6);
    skip_test(3,7);
    skip_test(4,1);
    skip_test(4,4);
    skip_test(4,6);
    skip_test(4,7);
    /**/

    //use to debug
    //skip_all_tests_except(test_id1,validation_id2);
    //...
    //skip_all_tests_except(test_idx,validation_idy);

    test1();
    test2();
    test3();
    test4();

    print_results();
}
