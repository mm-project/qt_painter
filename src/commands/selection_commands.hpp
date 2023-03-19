#ifndef selection_commands_hpp
#define selection_commands_hpp

#include "shape_creation_interactive_commands.hpp"

#include "../core/postman.hpp"
#include "../core/runtime_pool.hpp"
#include "../core/selection.hpp"
#include "../gui/statusbar_manager.hpp"

#include <QPoint>

#include <cassert>
#include <iostream>
#include <map>

class incmdSelectUnderCursoer : public InteractiveCommandBase
{
    bool m_move_mode = false;
    bool m_shape_added = false;
    RuntimePoolPtr m_sb;
    RuntimePoolManagerPtr m_re;
    ObjectPoolPtr m_ws;
    command_manager &m_cm = command_manager::getInstance();
    Selection &m_se = Selection::getInstance();
    IShapePtr m_original_shape = nullptr;
    LeCallback *m_sel_cb;
    bool m_need_mouserelase_log = false;

  public:
    incmdSelectUnderCursoer(RuntimePoolManagerPtr r, ObjectPoolPtr s) : m_re(r), m_ws(s)
    {
        // m_sb = std::shared_ptr<RuntimePool>(new RuntimePool);
        // m_re->addChild(m_sb,"aaa");
        m_sb = r->getChild("Generic-InteractiveCommand");
    }

    virtual void abort()
    {
        // FIXME now what?
    }

    virtual std::string get_name()
    {
        return "incmdSelectUnderCursoer";
    }

    virtual void execute()
    {
        InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(incmdSelectUnderCursoer, on_idle));
    }

    virtual bool need_log_mouserelease()
    {
        return true;
        return m_need_mouserelase_log;
    }

    /*void on_idle(const EvType& ev)
    {
        if ( ev == MM )
            if ( ! m_move_mode )
                m_se.highlight_shape_under_pos(InteractiveCommandBase::get_last_point());
            else
                move_selected_to_point(InteractiveCommandBase::get_last_point());
        else if ( ev == MC )
                on_click();
        else if ( ev == MP )
               on_press();
        else if ( ev == MR &&  m_move_mode )
                movement_commit();
    }*/

    void on_idle(const EvType &ev)
    {
        std::string msg("(idle) Selected " + QString::number(m_se.getObjects().size()).toStdString() + " shapes.");
        StatusBarManager::getInstance().updateStatusBar(msg.c_str(), 1, 0);

        // if ( ev == MU )
        //         on_click();
        // else
        if (ev == MM)
            m_se.highlight_shape_under_pos(InteractiveCommandBase::get_last_point());
        else if (ev == MD)
            on_press(OTHER); // set_next_handler(HANDLE_FUNCTION(incmdSelectUnderCursoer,on_press));
        else
            return;
    }

  private:
    void on_click()
    {
        m_se.clear();
        m_se.select_shape_under_pos(InteractiveCommandBase::get_last_point());
        std::string msg("Selected " + QString::number(m_se.getObjects().size()).toStdString() + " shapes.");
        StatusBarManager::getInstance().updateStatusBar(msg.c_str(), 1, 0);
    }

    void movement_commit()
    {
        // set_next_handler(HANDLE_FUNCTION(incmdSelectUnderCursoer,on_idle));
        if (m_original_shape == nullptr || m_se.getObjects().empty())
            return;

        RegionQuery &rq = RegionQuery::getInstance();

        // if (! m_se.getObjects().empty() )
        // m_ws->removeObject(dynamic_cast<WorkingSet*>((m_ws).get())->get_clonee(m_se.getObjects()[0]));

        m_move_mode = false;
        std::cout << "Adding..." << std::endl;

        // rq.removeObject(m_original_shape);
        int count = rq.getSize();
        m_ws->removeObject(m_original_shape);
        // temporary
        rq.clear();
        for (auto it : m_ws->getObjects())
            rq.insertObject(it);

        IShapePtr commited_obj = nullptr;
        for (auto it : m_sb->getObjects())
        {
            commited_obj = m_ws->addObject(it);
            rq.insertObject(commited_obj);
        }

        m_original_shape = nullptr;
        m_se.clear();
        m_se.addObjectFixme(commited_obj);
        m_sb->clear();
        m_se.temporary_highlight();
        // m_cm.return_to_idle();
        m_need_mouserelase_log = false;
        set_can_complete(true);
        set_next_handler(HANDLE_FUNCTION(incmdSelectUnderCursoer, on_idle));
    }

    void on_press(const EvType &)
    {
        // assert(0);
        // if (m_move_mode)
        //     return;
        // assert(0);
        /// if selection empty pick the shape under mouse
        // if ( m_se.getObjects().empty() )
        on_click();
        // if (!m_shape_added) {
        // if (m_sb)
        //     m_sb->clear();
        // m_se.highlightselect_shape_under_pos(InteractiveCommandBase::get_last_point());
        // m_se.select_shape_under_pos(InteractiveCommandBase::get_last_point());
        if (!m_se.getObjects().empty())
        {
            m_sb->clear();
            m_original_shape = m_se.getObjects()[0];
            // for ( auto it : m_se.getObjects() )
            //    m_sb->addObject(it);
            std::cout << "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << m_original_shape << std::endl;
            m_sb->addObject(m_original_shape);
            m_move_mode = true;
            // m_need_mouserelase_log = true;
            set_can_complete(false);
            set_next_handler(HANDLE_FUNCTION(incmdSelectUnderCursoer, on_mousemove));
        }
        // m_shape_added = true;
        //}
    }

    void on_mousemove(const EvType &ev)
    {
        if (ev == MM)
            move_selected_to_point(InteractiveCommandBase::get_last_point());
        else if (ev == MU)
            movement_commit();
    }

    void move_selected_to_point(QPoint p)
    {
        m_need_mouserelase_log = true;
        for (auto it : m_sb->getObjects())
        {
            // std::cout << "rotate..." << std::endl;
            it->moveCenterToPoint(p);
        }
    }
};

class dicmdSelectShapesByRegion : public DirectCommandBase
{
    std::pair<QPoint, QPoint> m_reg;

  public:
    dicmdSelectShapesByRegion()
    {
        add_option("-start", new PointCommandOptionValue());
        add_option("-end", new PointCommandOptionValue());
    }

    dicmdSelectShapesByRegion(QPoint p1, QPoint p2)
    {
        add_option("-start", new PointCommandOptionValue(p1));
        add_option("-end", new PointCommandOptionValue(p2));
    }

    virtual void execute()
    {

        m_reg = std::make_pair<QPoint, QPoint>(GET_CMD_ARG(PointCommandOptionValue, "-start"),
                                               GET_CMD_ARG(PointCommandOptionValue, "-end"));
        Selection::getInstance().clear();
        Selection::getInstance().find_and_highlightselect_shapes_from_region(m_reg);
        int count = Selection::getInstance().getObjects().size();
        std::string msg("Selected " + QString::number(count).toStdString() + " shapes.");
        StatusBarManager::getInstance().updateStatusBar(msg.c_str(), 1, 0);

        if (!Selection::getInstance().getObjects().empty())
        {
            LeCallbackData d;
            NOTIFY(OBJECT_SELECTED, d);
        }
    }

    virtual std::string get_name()
    {
        return "dicmdSelectShapesByRegion";
    }
};

class incmdSelectShapesByRegion : public incmdCreateObj<RECTANGLE>
{

  public:
    incmdSelectShapesByRegion(RuntimePoolManagerPtr r, ObjectPoolPtr s) : incmdCreateObj<RECTANGLE>(r, s)
    {
        m_first_click = true;
    }

    virtual std::string get_name()
    {
        // FIXME keep stringstream for converting int to str
        return "incmdSelectShapesByRegion";
    }

    virtual void execute()
    {
        StatusBarManager::getInstance().updateStatusBar("Click to select by region", 1, 0);

        ObjCreatorCommandBase<RECTANGLE>::set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion, on_idle));

        // FIXME how?
        /*
        ShapeProperties sp;
        sp.brush_color = Qt::gray;
        sp.pen_color = Qt::gray;
        sp.pen_style = Qt::DotLine;
        set_properties(sp);
        */
    }

    virtual void handle_update()
    {
        std::cout << "hu" << std::endl;
        // assert(0&&"applying properties to selection box?:)");
    }

    virtual void on_commit_internal()
    {
        // m_se.find_by_range_and_add_to_selected(m_reg);

        dicmdSelectShapesByRegion(m_reg.first, m_reg.second).silent_execute();
        incmdCreateObj<RECTANGLE>::finish();
        if (InteractiveCommandBase::is_auto_repeating())
            ObjCreatorCommandBase<RECTANGLE>::set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion, on_idle));
    }

    virtual void handle_mouse_click(int x, int y)
    {
        if (m_first_click)
            m_reg.first = InteractiveCommandBase::get_last_point();
        else
            m_reg.second = InteractiveCommandBase::get_last_point();

        m_first_click = true;
        InteractiveCommandBase::handle_mouse_click(x, y);
    }

    // helpers
  private:
    std::pair<QPoint, QPoint> m_reg;
    Selection &m_se = Selection::getInstance();
    bool m_first_click;

    // command cycle
  public:
    void on_idle(const EvType &ev)
    {
        if (ev == MC || ev == MD)
        {
            if (m_first_click)
            {
                m_first_click = false;
                m_se.clear();
            }

            incmdCreateObj<RECTANGLE>::idle(ev);

            // FIXME doesn't work!!!
            //*
            /*
            if ( incmdCreateObj<RECTANGLE>::idle(ev) ) {
                    //std::cout << "setting.." << std::endl;
                    IShape* s = incmdCreateObj<RECTANGLE>::get_runtime_object();
                    assert(s);

                    ShapeProperties sp;
                    sp.brush_color = Qt::blue;
                    sp.pen_color = Qt::blue;
                    sp.pen_style = Qt::DotLine;

                    //s->updateProperties(sp);
            }
            */
        }
    }
};

/*
class incmdSelectShapesByRegion : public ObjCreatorCommandBase<RECTANGLE>
{

public:
        incmdSelectShapesByRegion(ObjectPoolSandboxPtr r, IObjectPoolPtr s ):ObjCreatorCommandBase<RECTANGLE>(r,s)
    {
    }


        virtual std::string get_name() {
                //FIXME keep stringstream for converting int to str
                return "incmdSelectShapesByRegion";
        }

        virtual void execute() {
                m_se = Selection::getInstance();
                m_se.clear();
                ObjCreatorCommandBase<RECTANGLE>::set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_idle));
        }

        virtual void abort() {
                //FIXME now what?
        }


//helpers
private:
        std::pair<QPoint,QPoint> m_reg;
        Selection* m_se;

//command cycle
public:
        void on_idle(const EvType& ev) {
                if ( ev != MC )
                        return;

                m_se.clear();
                m_reg.first = InteractiveCommandBase::get_last_point();
                set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_first_click));
        }

        void on_first_click(const EvType& ev) {
                if ( ev != MC )
                        return;

                m_reg.second = InteractiveCommandBase::get_last_point();
                set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_finish));
        }

        void on_finish(const EvType&) {
                //std::cout << "interactive command COMMIT..." << std::endl;
                m_se.find_by_range_and_add_to_selected(m_reg);
                set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_idle));
        }
};
*/

#endif
