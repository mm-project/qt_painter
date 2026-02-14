#ifndef DELETE_COMMAND_HPP
#define DELETE_COMMAND_HPP

#include "command_manager.hpp"
#include "direct_command_base.hpp"
#include "interactive_command_base.hpp"

#include "../core/design.hpp"
#include "../core/rq/RegionQueryService.hpp"
#include "../core/runtime_pool.hpp"
#include "../gui/statusbar_manager.hpp"

class dicmdDeleteObj : public DirectCommandBase
{
  public:
    dicmdDeleteObj(ObjectPoolPtr ptr, QPoint pos) : m_workingSet(std::dynamic_pointer_cast<Design>(ptr))
    {
        add_option("-point", new PointCommandOptionValue(pos));
    }

    dicmdDeleteObj(ObjectPoolPtr ptr) : m_workingSet(std::dynamic_pointer_cast<Design>(ptr))
    {
        add_option("-point", new PointCommandOptionValue());
    }

    virtual std::string get_name() override
    {
        return "dicmdDeleteShape";
    }

    virtual ICommandResult* execute() override
    {
        RegionQuery &rq = RegionQuery::getInstance();
        QPoint pos = GET_CMD_ARG(PointCommandOptionValue, "-point");
        auto shapes = rq.getShapeUnderPos(pos);
        //rq.clear();

        //for (auto& obj : m_workingSet->getObjects())
        //	rq.insertObject(obj);

        for ( auto& shape : shapes )
        {
            if (shape != nullptr)
            {
                rq.removeObject(shape);
                m_workingSet->removeObject(shape);
            }
        }
    }

  private:
    DesignPtr m_workingSet = nullptr;
};

class InteractiveDeleteAction : public InteractiveCommandBase
{
  public:
    InteractiveDeleteAction(ObjectPoolPtr ptr) : m_workingSet(std::dynamic_pointer_cast<Design>(ptr))
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

    virtual ICommandResult* execute() override
    {
        StatusBarManager::getInstance().updateStatusBar("Click and delete object", 1, 0);
        InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(InteractiveDeleteAction, idle));
    }

    void on_commit(const EvType &)
    {
        dicmdDeleteObj(m_workingSet, m_position).silent_execute();
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
    DesignPtr m_workingSet = nullptr;
};


class dicmdDeleteObj2 : public DirectCommandBase
{

    ObjectPoolPtr ws;
    RegionQuery &rq = RegionQuery::getInstance();

  public:
    dicmdDeleteObj2(ObjectPoolPtr s) : ws(s)
    { 
        add_option("-object_id", new IntCommandOptionValue(0));
    }

    dicmdDeleteObj2(ObjectPoolPtr s, int object_id) : ws(s)
    {
        add_option("-object_id", new IntCommandOptionValue(object_id));
    }

    virtual ICommandResult* execute()
    {
        int object_id = GET_CMD_ARG(IntCommandOptionValue, "-object_id");
        auto object = global_fixme1[object_id];

        rq.removeObject(object);
        ws->removeObject(object);
        return 0;
    }

    virtual std::string get_name()
    {
        return "dicmdDeleteObj2";
    }
};

#endif
