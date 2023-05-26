#include "selection.hpp"
#include "../gui/controller.hpp"
#include "postman.hpp"
#include "rq/RegionQueryService.hpp"

#include <QDir>
#include <QFile>
#include <QTextStream>

std::string Selection::getName() const noexcept
{
    return "Selection";
}

// shold be removed after Highlit change
void Selection::highlightselect_all()
{
    clear();
    for (const auto& obj : m_ws->getObjects())
    {
        addObject(obj);
        //m_sel_highlight_set->addObject(obj);
    }
    //m_sel_highlight_set->highlight_on();
}

void Selection::temporary_highlight()
{
    //m_sel_highlight_set->highlight_on();
}

void Selection::addObjectFixme(IShapePtr p)
{
    addObject(p);
    //m_sel_highlight_set->addObject(p);
}

void Selection::clear() noexcept
{
    ObjectPoolBase::clear();
    //m_sel_highlight_set->clear();
    //m_ao_highlight_set->clear();
    // m_sb->clear();
}

void Selection::set_working_set(ObjectPoolPtr ws)
{
    m_ws = ws;
    // m_h_on = false;
    REGISTER_CALLBACK(CONTROLLER_CHANGED, &Selection::on_controller_update);
}

void Selection::on_controller_update(LeCallbackData &)
{
    std::cout << "changed.." << std::endl;

    if (m_ws->getObjects().empty() || getObjects().empty())
        return;

    for (auto obj : getObjects())
    {
        obj->updateProperties(controller::getInstance().get_shape_properties());
        // obj->updateProperties(controller::getInstance().get_shape_properties());
    }
}

void Selection::set_sandbox(RuntimePoolManagerPtr sanboxes)
{
    m_rt_pools = sanboxes;
    // m_sb = new RuntimePool();
    // m_rt_pools->addChildren(std::shared_ptr<RuntimePool>(m_sb));

    ShapeProperties p1;
    p1.pen_color = Qt::yellow;
    p1.brush_color = Qt::blue;
    p1.pen_style = Qt::DotLine;
    p1.brush_style = Qt::DiagCrossPattern;
    p1.pen_width = 5;

    ShapeProperties p2;
    p2.pen_color = Qt::red;
    p2.pen_style = Qt::DashDotLine;
    p2.pen_width = 1;
    p2.brush_style = Qt::NoBrush;

    ShapeProperties p3;
    p3.pen_color = Qt::white;
    p3.brush_style = Qt::NoBrush;
    p3.pen_style = Qt::DashDotLine;
    p3.pen_width = 1;

    m_ao_highlight_set = new HighlightSet("ActiveObjects", p1);
    m_sel_highlight_set = new HighlightSet("Selection", p2);
    m_qa_highlight_set = new HighlightSet("QASelectionsTintirid", p3);

    //m_sel_highlight_set->create_sandbox(m_rt_pools);
    //m_ao_highlight_set->create_sandbox(m_rt_pools);
    m_qa_highlight_set->create_sandbox(m_rt_pools);
}

// asenq te
void Selection::highlightselect_shape_under_pos(const QPoint &p)
{
    clear();
    IShapePtr shape = std::shared_ptr<IShape>(rq.getShapeUnderPos(p));
    if (shape != nullptr)
    {
        addObject(shape);
        //m_sel_highlight_set->addObject(shape);
        //m_sel_highlight_set->highlight_on();
    }
}

// just highlight active object
void Selection::highlight_shape_under_pos(const QPoint &p)
{
    IShapePtr shape = std::shared_ptr<IShape>(rq.getShapeUnderPos(p));
    //m_ao_highlight_set->clear();
    if (shape != nullptr)
    {
        //m_ao_highlight_set->addObject(shape);
        //m_ao_highlight_set->highlight_on();
    }
}

/* ! get shapes from rq that under following coord
 *   and add them to local set ( vector ).
 ! */
void Selection::select_shape_under_pos(const QPoint &p)
{
    // clear();
    IShapePtr shape = rq.getShapeUnderPos(p);
    if (shape != nullptr)
    {
        addObject(shape);
    }
}

/* ! get shapes from rq under following bbox, that are part of working set
 *   and add them to local set ( vector ).
 ! */
void Selection::find_and_highlightselect_shapes_from_region(const std::pair<QPoint, QPoint> &point)
{
    clear();
    if (m_ws->getObjects().empty())
        return;

    for (auto it : rq.getShapesUnderRect(QRect(point.first, point.second)))
    {
        addObject(it);
        //m_sel_highlight_set->addObject(it);
    }

    m_last_region = QRect(point.first, point.second);
    //m_sel_highlight_set->highlight_on();
}

void Selection::highlight_last_selected_region(bool on_off)
{
    // std::cout << "hiiiiiiiiiiiiiiiiiiiiii" << std::endl;
    if (!on_off)
    {
        m_qa_highlight_set->clear();
        m_last_region = QRect(0, 0, 0, 0);
        return;
    }

    std::cout << m_last_region.bottomRight().x() << " " << m_last_region.bottomRight().y() << std::endl;
    // use shape creator instead of clone
    m_qa_highlight_set->addObject(std::shared_ptr<IShape>(Rectangle(m_last_region, ShapeProperties()).clone()));
    m_qa_highlight_set->highlight_on();
}

//*********************************** HIGHLIGHT ********************************************
HighlightSet::HighlightSet(const std::string &n, const ShapeProperties &p) : m_name(n), m_packet(p)
{
}

void HighlightSet::create_sandbox(RuntimePoolManagerPtr ops)
{
    m_rt_pools = ops;
    m_sb = std::shared_ptr<RuntimePool>(new RuntimePool());
    m_rt_pools->addChild(m_sb, "Highlight-" + m_name);
    // m_sb = m_rt_pools->getChild("Canvas");
}

std::string HighlightSet::getName() const noexcept
{
    return m_name;
}

void HighlightSet::highlight_on()
{
    highlight_on_off(true);
}

void HighlightSet::clear() noexcept
{
    ObjectPoolBase::clear();
    m_sb->clear();
    // highlight_off();
}

void HighlightSet::highlight_off()
{
    clear();
    // highlight_on_off(false);
}

void HighlightSet::highlight_on_off(bool)
{
    // check with levon
    if (getObjects().empty())
        return;

    for (auto it : getObjects())
    {
        m_sb->addObject(it);
        m_sb->changeBasicProperties(m_packet);
    }
}
