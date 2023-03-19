#ifndef renderer_hpp
#define renderer_hpp

#include "callback.hpp"
#include "iobject_pool.hpp"
#include "postman.hpp"
#include "runtime_pool.hpp"
#include "selection.hpp"

#include <QPainter>
#include <QPoint>
#include <QWidget>

#include <iostream>

enum panDirection
{
    PANUP,
    PANDOWN,
    PANLEFT,
    PANRIGHT
};
enum zoomDirection
{
    ZOOMIN,
    ZOOMOUT
};

namespace
{
std::string panDirection2str(const panDirection &p)
{
    switch (p)
    {
    case PANUP:
        return "PanUp";
    case PANDOWN:
        return "PanDown";
    case PANLEFT:
        return "PanLeft";
    case PANRIGHT:
        return "PanRight";
    }
    return "";
}

std::string zoomDirection2str(const zoomDirection &p)
{
    switch (p)
    {
    case ZOOMIN:
        return "ZoomIn";
    case ZOOMOUT:
        return "ZoomOut";
    }
    return "";
}
} // namespace

class canvasTransformClbkDt : public LeCallbackData
{
  public:
    canvasTransformClbkDt(float kx, int dx, float ky, int dy) : m_kx(kx), m_ky(ky), m_dx(dx), m_dy(dy)
    {
    }

    float m_kx = 1;
    float m_ky = 1;
    int m_dx = 0;
    int m_dy = 0;
};

// REPONSIBLE FOR VIEWPORT CONTROLL
class renderer
{
    // Q_OBJECT
  public:
    renderer(QWidget *w, RuntimePoolManagerPtr r, ObjectPoolPtr s);
    ~renderer();
    void render();
    void pan(const panDirection &d);

    void zoom_internal(const zoomDirection &z, QPoint p);

    void zoom(int factor, QPoint p);
    void zoomin_p(QPoint p);
    void zoomout_p(QPoint p);
    void set_cursor_pos_for_drawing(int, int);
    void click_hint();
    void rendering_mode_change();
    void rendering_rt_mode_change();
    void rendering_des_mode_change();

  private:
    QPainter *get_painter();
    void start();
    void stop();
    void notify_viewport_changed();
    float get_zoom_factor();

    void prezoom(QPoint p);
    void zoomin();
    void zoomout();

    void make_viewport_adjustments();

    void draw_background();
    void draw_grid();
    void draw_objects();
    void draw_runtime_pools();
    void draw_cursor();
    void draw_all();
    // void draw_selection_rubberband();
    void draw_all_wno_cursor();

  private:
    Selection &m_se = Selection::getInstance();

  private:
    float m_scale_factor = 1;
    float m_zoom_factor = 1.1;

  private:
    bool m_rq_renderer = true;
    bool m_rt_renderer = true;
    bool m_des_renderer = true;

    QPainter *m_qt_painter;
    ObjectPoolPtr m_working_set;
    RuntimePoolManagerPtr m_sandbox;
    int m_scale = 30;
    int m_pan_step = 10;
    QPoint m_origin_point = {0, 0};
    QPoint m_old_origin_point = {0, 0};
    bool m_need_adjustment = false;
    QWidget *m_plane;
    int c_cursor_x = 0;
    int c_cursor_y = 0;
    bool m_need_draw_clicked = false;
    QRect *m_users_pov_rect;
};

#endif
