#ifndef DELETE_COMMAND_HPP
#define DELETE_COMMAND_HPP

#include "command_manager.hpp"
#include "direct_command_base.hpp"
#include "interactive_command_base.hpp"

#include "../core/design.hpp"
#include "../core/rq/RegionQueryService.hpp"
#include "../gui/statusbar_manager.hpp"
#include "../core/design/design_manager.hpp"

class dicmdDeleteObj : public DirectCommandBase
{
  public:
    dicmdDeleteObj(QPoint pos) 
    {
        add_option("-point", new PointCommandOptionValue(pos));
    }

    dicmdDeleteObj() 
    {
        add_option("-point", new PointCommandOptionValue());
    }

    virtual std::string get_name() override
    {
        return "dicmdDeleteShape";
    }

    virtual void execute() override
    {
        RegionQuery &rq = RegionQuery::getInstance();
        QPoint pos = GET_CMD_ARG(PointCommandOptionValue, "-point");
        auto shapes = rq.getShapeUnderPos(pos);
        //rq.clear();

        //for (auto& obj : m_workingSet->getObjects())
        //	rq.insertObject(obj);

        auto& dm = DesignManager::getInstance();
        auto pActiveDesign = dm.getActiveDesign();
        for ( auto& shape : shapes )
        {
            if (shape != nullptr)
            {
                rq.removeObject(shape);
                pActiveDesign->removeObject(shape);
            }
        }
    }

  private:
};

class InteractiveDeleteAction : public InteractiveCommandBase
{
  public:
    InteractiveDeleteAction() : m_position()
    {
    }

    virtual std::string get_name() override
    {
        return "incmdDeleteShape";
    }

    virtual void abort() override
    {
        command_manager::getInstance().return_to_idle();
    }

    virtual void execute() override
    {
        StatusBarManager::getInstance().updateStatusBar("Click and delete object", 1, 0);
        InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(InteractiveDeleteAction, idle));
    }

    void on_commit(const EvType &)
    {
        dicmdDeleteObj(m_position).silent_execute();
        InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(InteractiveDeleteAction, idle));
    }

    void idle(const EvType &)
    {
        // if (ev != MC)
        //	return;

        InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(InteractiveDeleteAction, on_click));
    }

    void on_click(const EvType &ev)
    {
        // not from me
        if (ev == MC || ev == MD)
        {
            m_position = InteractiveCommandBase::get_last_point();
            on_commit(OTHER);
        }
    }

  private:
    QPoint m_position;
};
#endif
