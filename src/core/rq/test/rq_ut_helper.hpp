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
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>
#include <algorithm>


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


int ws_insert_ms = 999;
int rq_insert_ms = 888;
int ws_query_ms = 777;
int rq_query_ms = 666;



std::vector<IShapePtr> getShapesUnderRect(int x,int y,int w,int h) 
{
    
    std::vector<IShapePtr> shapes;    
    QRect r = QRect(x,y,w,h);

    for (auto shape : the_ws)
        if (shape->intersects(r))
            shapes.push_back(shape);

    return shapes;
}


template <typename T> IShapePtr create(QPoint p1, QPoint p2)
{
    IShapePtr shape = std::shared_ptr<T>(new T);
    shape->addPoint(p2);
    shape->addPoint(p1);

    return shape;
}

template <typename T> IShapePtr create1(std::vector<QPoint> points)
{
    IShapePtr shape = std::shared_ptr<T>(new T);
    for (auto & p: points)
        shape->addPoint(p);

    return shape;
}

template <typename T> void create_obj(int x1, int y1, int x2, int y2, bool ws)
{
    // remove if need to test negative coordinates as well 
    if ( x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 )
        return;

    std::vector<QPoint> points;
    points.push_back(QPoint(x1, y1));
    points.push_back(QPoint(x2, y2));

    if constexpr (std::is_same_v<T, Polygon>) {
        points.push_back(QPoint(x1, y2));
        points.push_back(QPoint(x2, y1));
    }

    //auto rect = create<T>(QPoint(x1, y1), QPoint(x2, y2));
    auto rect = create1<T>(points);
    if (ws) 
        the_ws.push_back(rect);
    else
        rq.insertObject(rect);
}

template <typename T> void create_obj_at_given_cell_and_row(int column, int row, bool ws)
{
    // don't know, but it does what i wanted
    int delta = 100;
    int epsilon = 90;
    create_obj<T>(delta*row,delta*column,delta*row-epsilon,delta*column-epsilon,ws);
}

template <typename T> void insert_nxn_matrix_of_objs_internal(int n, bool ws)
{
    int total = n*n;
    double i = 1;
    double incr = 10;
    double percent = 0;
    for(int column=0; column<n; column++) {
        for(int row=0; row<n; row++) {
            create_obj_at_given_cell_and_row<T>(column,row, ws);
            i++;
            double diff = total - i;
            percent = 100 - (diff * 100)/total;
            if (fmod(percent,incr) == 0.0)
                std::cout << "  "  << percent << "%" << std::flush; 
                //std::cout << "  "  << total << " " << i << " " << percent << "%" << std::endl;
        }
    }
    std::cout << std::endl;
}

template <typename T> void insert_nxn_matrix_of_objs(int n)
{
    std::cout << "  inserting " << n << "x" << n << " objects to ws ..." << std::endl;
    the_ws.reserve(n*n);
    auto ws_insert_start = std::chrono::high_resolution_clock::now();
    insert_nxn_matrix_of_objs_internal<T>(n,true);
    auto ws_insert_end = std::chrono::high_resolution_clock::now();
    ws_insert_ms = std::chrono::duration_cast<std::chrono::microseconds>(ws_insert_end - ws_insert_start).count();
    std::cout << "       --> ws insert took " << ws_insert_ms <<  std::endl;

    
    std::cout << "  inserting " << n << "x" << n << " objects to rq ..." << std::endl;
    auto rq_insert_start = std::chrono::high_resolution_clock::now();
    insert_nxn_matrix_of_objs_internal<T>(n,false);
    auto rq_insert_end = std::chrono::high_resolution_clock::now();
    rq_insert_ms = std::chrono::duration_cast<std::chrono::microseconds>(rq_insert_end - rq_insert_start).count();
    std::cout << "       --> rq insert took " << rq_insert_ms <<  std::endl;

}

auto init() 
{
    std::cout <<"[test " << test_id<< "]: initizialazing ..." << std::endl;
    rq.clear();
    the_ws.clear();
    
    /*
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
    */
}

void fini()
{
    /*
    delete drawbles.first.painter;
    delete drawbles.first.pixmap;
    delete drawbles.second.painter;
    delete drawbles.second.pixmap;
    
    drawbles.first.pixmap = 0;
    drawbles.second.pixmap = 0;
    drawbles.first.painter = 0;
    drawbles.second.painter = 0;
    */
    std::cout << std::endl;
    rq.clear();
    the_ws.clear();

    test_id++;
    validation_id = 1;
}

void refresh_drawbles()
{
    return;

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

void validate_rq(int x, int y, int width, int height, bool need_drawing = true)
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
    auto rq_duration = std::chrono::duration_cast<std::chrono::microseconds>(rq_end - rq_start).count();
    std::cout << "       --> rq lookup took " << rq_duration << "ms and got " << rq_shapes.size() << " objects" << std::endl;
    rq_query_ms = rq_duration;

    auto ws_start = std::chrono::high_resolution_clock::now();
    auto ws_shapes = getShapesUnderRect(x, y, width, height);
    auto ws_end = std::chrono::high_resolution_clock::now();
    auto ws_duration = std::chrono::duration_cast<std::chrono::microseconds>(ws_end - ws_start).count();
    std::cout << "       --> ws lookup took " << ws_duration << "ms and got " << ws_shapes.size() << " objects" << std::endl;
    ws_query_ms = ws_duration;

    if (rq_shapes.size() != ws_shapes.size()) {
        std::cout << "       Issue found!" << std::endl;
        failed_tests.push_back(test_info(test_id,validation_id,rq_shapes.size(),ws_shapes.size(),QRect(x, y, width, height)));
    } else {
        std::cout << "       passed" << std::endl;
    }

    if ( need_drawing ) {
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
    }
    
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

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>
#include <algorithm>

void write_results_graph_html(const std::map<int, std::vector<int>>& results)
{
    struct Pt { int x; int y; };
    // Series
    std::vector<Pt> wsInsert, rqInsert, wsQuery, rqQuery;

    // Global X range (magnitude)
    int x_min = std::numeric_limits<int>::max();
    int x_max = std::numeric_limits<int>::min();

    auto upd = [](int v, int& lo, int& hi){ lo = std::min(lo, v); hi = std::max(hi, v); };

    // Per-chart Y "tops" (for suggestedMax). Each chart has its own scale.
    int y_ins_both_top = std::numeric_limits<int>::min();
    int y_qry_both_top = std::numeric_limits<int>::min();
    int y_ins_ws_top   = std::numeric_limits<int>::min();
    int y_ins_rq_top   = std::numeric_limits<int>::min();
    int y_qry_ws_top   = std::numeric_limits<int>::min();
    int y_qry_rq_top   = std::numeric_limits<int>::min();

    for (const auto& [mag, vec] : results) {
        upd(mag, x_min, x_max);

        auto push_top = [&](size_t idx, std::vector<Pt>& dst, int& chart_top){
            if (vec.size() > idx) {
                int d = vec[idx];
                dst.push_back({mag, d});
                chart_top = std::max(chart_top, d);
            }
        };

        // Map indices → series
        push_top(0, wsInsert, y_ins_ws_top);
        push_top(1, rqInsert, y_ins_rq_top);
        push_top(2, wsQuery,  y_qry_ws_top);
        push_top(3, rqQuery,  y_qry_rq_top);

        // Row 1 combined chart tops
        y_ins_both_top = std::max({y_ins_both_top, y_ins_ws_top, y_ins_rq_top});
        y_qry_both_top = std::max({y_qry_both_top, y_qry_ws_top, y_qry_rq_top});
    }

    if (x_min == std::numeric_limits<int>::max()) { x_min = 0; x_max = 1; }
    auto safeTop = [](int v){ return (v==std::numeric_limits<int>::min()) ? 1 : v; };

    auto to_js = [](const std::vector<Pt>& v){
        std::ostringstream os; os << "[";
        for (size_t i=0;i<v.size();++i){
            if(i) os<<",";
            os<<"{\"x\":"<<v[i].x<<",\"y\":"<<v[i].y<<"}";
        }
        os << "]"; return os.str();
    };

    std::ofstream out("results_graph.html");
    out <<
R"(<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <title>Insert & Query — WS vs RQ</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    * { box-sizing: border-box; }
    body { margin: 0; font-family: system-ui, -apple-system, Segoe UI, Roboto, sans-serif; background:#fafafa; }
    h2 { text-align: center; margin: 20px 0; }
    .charts { display: grid; grid-template-columns: 1fr 1fr; gap: 24px; padding: 8px 24px 24px; }
    .chart-card { position: relative; aspect-ratio: 3 / 2; min-height: 360px; background: #fff; border-radius: 12px; box-shadow: 0 1px 4px rgba(0,0,0,.08); }
    .chart-card > canvas { position: absolute; inset: 0; width: 100%; height: 100%; }
    .row-title { margin: 6px 24px 0; font-weight: 600; color:#333; }
    @media (max-width: 1100px) { .charts { grid-template-columns: 1fr; } }
  </style>
</head>
<body>
  <h2>Insert & Query — WS vs RQ</h2>

  <!-- Row 1: Insert (WS & RQ) — Query (WS & RQ) -->
  <div class="row-title">Insert (WS & RQ) — Query (WS & RQ)</div>
  <div class="charts">
    <div class="chart-card"><canvas id="insert_both"></canvas></div>
    <div class="chart-card"><canvas id="query_both"></canvas></div>
  </div>

  <!-- Row 2: Insert (WS) — Insert (RQ) -->
  <div class="row-title">Insert (WS) — Insert (RQ)</div>
  <div class="charts">
    <div class="chart-card"><canvas id="insert_ws"></canvas></div>
    <div class="chart-card"><canvas id="insert_rq"></canvas></div>
  </div>

  <!-- Row 3: Query (WS) — Query (RQ) -->
  <div class="row-title">Query (WS) — Query (RQ)</div>
  <div class="charts">
    <div class="chart-card"><canvas id="query_ws"></canvas></div>
    <div class="chart-card"><canvas id="query_rq"></canvas></div>
  </div>

  <script>
    // ===== Data from C++ =====
    const wsInsert = )" << to_js(wsInsert) << R"(;
    const rqInsert = )" << to_js(rqInsert) << R"(;
    const wsQuery  = )" << to_js(wsQuery)  << R"(;
    const rqQuery  = )" << to_js(rqQuery)  << R"(;

    const xMin = )" << x_min << R"(;
    const xMax = )" << x_max << R"(;

    // Per-chart suggestedMax (never go below 0 thanks to suggestedMin)
    const yInsBothMax = )" << safeTop(y_ins_both_top) << R"( * 1.05;
    const yQryBothMax = )" << safeTop(y_qry_both_top) << R"( * 1.05;
    const yInsWsMax   = )" << safeTop(y_ins_ws_top)   << R"( * 1.05;
    const yInsRqMax   = )" << safeTop(y_ins_rq_top)   << R"( * 1.05;
    const yQryWsMax   = )" << safeTop(y_qry_ws_top)   << R"( * 1.05;
    const yQryRqMax   = )" << safeTop(y_qry_rq_top)   << R"( * 1.05;

    const fmt = n => n.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ',');

    function makeOptions(title, yTop){
      return {
        responsive: true,
        maintainAspectRatio: true,
        interaction: { mode: 'nearest', intersect: false },
        plugins: {
          legend: { position: 'top' },
          title: { display: true, text: title }
        },
        scales: {
          x: { min: xMin, max: xMax, title: { display: true, text: 'Magnitude' } },
          y: { suggestedMin: 0, suggestedMax: yTop, title: { display: true, text: 'Duration (ms)' }, ticks: { callback: v => fmt(v) } }
        }
      };
    }

    // ==== Row 1 ====
    new Chart(document.getElementById('insert_both'), {
      type: 'scatter',
      data: { datasets: [
        { label: 'WS Insert', data: wsInsert, borderColor: 'blue',  backgroundColor: 'rgba(0,0,255,0.15)', showLine: true, pointRadius: 3 },
        { label: 'RQ Insert', data: rqInsert, borderColor: 'red',   backgroundColor: 'rgba(255,0,0,0.15)', showLine: true, pointRadius: 3 }
      ]},
      options: makeOptions('Insert (WS & RQ)', yInsBothMax)
    });

    new Chart(document.getElementById('query_both'), {
      type: 'scatter',
      data: { datasets: [
        { label: 'WS Query', data: wsQuery, borderColor: 'blue', backgroundColor: 'rgba(0,0,255,0.15)', showLine: true, pointRadius: 3 },
        { label: 'RQ Query', data: rqQuery, borderColor: 'red',  backgroundColor: 'rgba(255,0,0,0.15)', showLine: true, pointRadius: 3 }
      ]},
      options: makeOptions('Query (WS & RQ)', yQryBothMax)
    });

    // ==== Row 2 ====
    new Chart(document.getElementById('insert_ws'), {
      type: 'scatter',
      data: { datasets: [
        { label: 'WS Insert', data: wsInsert, borderColor: 'blue', backgroundColor: 'rgba(0,0,255,0.15)', showLine: true, pointRadius: 3 }
      ]},
      options: makeOptions('Insert (WS)', yInsWsMax)
    });

    new Chart(document.getElementById('insert_rq'), {
      type: 'scatter',
      data: { datasets: [
        { label: 'RQ Insert', data: rqInsert, borderColor: 'red', backgroundColor: 'rgba(255,0,0,0.15)', showLine: true, pointRadius: 3 }
      ]},
      options: makeOptions('Insert (RQ)', yInsRqMax)
    });

    // ==== Row 3 ====
    new Chart(document.getElementById('query_ws'), {
      type: 'scatter',
      data: { datasets: [
        { label: 'WS Query', data: wsQuery, borderColor: 'blue', backgroundColor: 'rgba(0,0,255,0.15)', showLine: true, pointRadius: 3 }
      ]},
      options: makeOptions('Query (WS)', yQryWsMax)
    });

    new Chart(document.getElementById('query_rq'), {
      type: 'scatter',
      data: { datasets: [
        { label: 'RQ Query', data: rqQuery, borderColor: 'red', backgroundColor: 'rgba(255,0,0,0.15)', showLine: true, pointRadius: 3 }
      ]},
      options: makeOptions('Query (RQ)', yQryRqMax)
    });

  </script>
</body>
</html>)";
}

int get_mean_value(std::vector<int> vec) 
{
    if (vec.empty()) return 0; // or throw an exception if preferred

    std::sort(vec.begin(), vec.end());
    size_t n = vec.size();

    if (n % 2 == 1) {
        return vec[n / 2];
    } else {
        return (vec[n / 2 - 1] + vec[n / 2]) / 2; // Integer division
    }
}

void do_perf_test(int repeat_factor, std::vector<int>& magnitudes)
{
    std::map<int, std::vector<int>> results;
    for (auto & magnitude: magnitudes) {
        std::vector<int> ws_insert_times;
        std::vector<int> rq_insert_times;
        std::vector<int> ws_query_times;
        std::vector<int> rq_query_times;
        for (int i=0; i<repeat_factor; i++) {
            init();
            insert_nxn_matrix_of_objs<Rectangle>(magnitude);
            validate_rq(400, 400, 400, 400, false);
            fini();
            ws_insert_times.push_back(ws_insert_ms);
            rq_insert_times.push_back(rq_insert_ms);
            ws_query_times.push_back(ws_query_ms);
            rq_query_times.push_back(rq_query_ms);
        }
        int ws_insert_mean_ms = get_mean_value(ws_insert_times);
        int rq_insert_mean_ms = get_mean_value(rq_insert_times);
        int ws_query_mean_ms = get_mean_value(ws_query_times);
        int rq_query_mean_ms = get_mean_value(rq_query_times);
        
        std::vector<int> data;
        data.push_back(ws_insert_mean_ms);
        data.push_back(rq_insert_mean_ms);
        data.push_back(ws_query_mean_ms);
        data.push_back(rq_query_mean_ms);
        results[magnitude] = data;
    }

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "********* PERF TESTING *************" << std::endl;
    std::cout << std::endl;
    std::cout << "magnitude, ws_insert, rq_insert, ws_query, rq_query" << std::endl;
    for (auto& [key, data] : results) {    
        std::cout << key << "," << data[0] << "," << data[1] << "," << data[2] << "," << data[3] << std::endl;
        //std::cout << "[ N = " << key << " ]" << std::endl;
        /*std::cout << "  insert:" << std::endl;
        std::cout << "     ws: " << ws_insert_mean_ms << std::endl;
        std::cout << "     rq: " << rq_insert_mean_ms << std::endl;
        std::cout << "  query:" << std::endl;
        std::cout << "     ws: " << ws_query_mean_ms << std::endl;
        std::cout << "     rq: " << rq_query_mean_ms << std::endl;
        */
        //std::cout << std::endl;    
    }
    std::cout << std::endl;
    std::cout << "Calculating computional complexity" << std::endl;
    int j = 1;
    std::map<int, std::vector<int>> seqs;
    for (int i=0; i<magnitudes.size()-1; i++) {
        std::vector<int> results_1 = results[magnitudes[i+1]];
        std::vector<int> results_2 = results[magnitudes[i]];
        auto mag_ratio = magnitudes[i+1]/magnitudes[i];
        auto ws_insert_ratio = results_1[0]/results_2[0];
        auto rq_insert_ratio = results_1[1]/results_2[1];
        auto ws_query_ratio = 1;
        if ( results_2[2] != 0 )
            ws_query_ratio = results_1[2]/results_2[2];
        auto rq_query_ratio = 1;
        if ( results_2[3] != 0 )
            rq_query_ratio = results_1[3]/results_2[3];

        std::cout << "step "<< j << std::endl;
        std::cout << "  mag ratio:" << mag_ratio << std::endl; 
        std::cout << "  ws insert ratio:" << ws_insert_ratio << std::endl; 
        seqs[0].push_back(ws_insert_ratio);
        std::cout << "  rq insert ratio:" << rq_insert_ratio << std::endl; 
        seqs[1].push_back(rq_insert_ratio);
        std::cout << "  ws query ratio:" << ws_query_ratio << std::endl; 
        seqs[2].push_back(ws_query_ratio);
        std::cout << "  rq query ratio:" << rq_query_ratio << std::endl; 
        seqs[3].push_back(rq_query_ratio);    
        j++;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    
    std::cout << "ws insert sequence: ";
    for (auto & ws_insert: seqs[0])
        std::cout << ws_insert << ",";
    std::cout << std::endl;
    
    std::cout << "rq insert sequence: ";
    for (auto & rq_insert: seqs[1])
        std::cout << rq_insert << ",";
    std::cout << std::endl;

    std::cout << "ws query sequence: ";
    for (auto & ws_query: seqs[2])
        std::cout << ws_query << ",";
    std::cout << std::endl;

    std::cout << "rq query sequence: ";
    for (auto & rq_query: seqs[3])
        std::cout << rq_query << ",";
    std::cout << std::endl;

    std::cout << std::endl;    
    write_results_graph_html(results);

}
