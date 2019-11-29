#ifndef DELETE_COMMAND_HPP
#define DELETE_COMMAND_HPP

#include "interactive_command_base.hpp"
#include "command_manager.hpp"
#include "direct_command_base.hpp"

#include "../core/rq/RegionQueryService.hpp"
#include "../gui/statusbar_manager.hpp"



class dicmdDeleteObj : public DirectCommandBase
{
public:
	dicmdDeleteObj(IObjectPoolPtr ptr, QPoint pos)
		: m_workingSet(std::dynamic_pointer_cast<WorkingSet>(ptr))
	{
		add_option("-point",new PointCommandOptionValue(pos));
	}

	dicmdDeleteObj(IObjectPoolPtr ptr)
		: m_workingSet(std::dynamic_pointer_cast<WorkingSet>(ptr))
	{
		add_option("-point",new PointCommandOptionValue());
	}

	virtual std::string get_name() override
	{
		return "dicmdDeleteShape";
	}

	virtual void execute() override
	{
		RegionQuery& rq = RegionQuery::getInstance();
		QPoint pos = GET_CMD_ARG(PointCommandOptionValue,"-point");
		IShape* shape = rq.getShapeUnderPos(pos);
		if (shape != nullptr)
		{
			rq.removeObject(shape);
			m_workingSet->removeObject(shape);
		}

	}

private:
	WorkingSetPtr m_workingSet = nullptr;
};

class InteractiveDeleteAction : public InteractiveCommandBase
{
public:
	InteractiveDeleteAction(IObjectPoolPtr ptr)
		: m_workingSet(std::dynamic_pointer_cast<WorkingSet>(ptr))
	{}

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
		StatusBarManager::getInstance().updateStatusBar("Click and delete object",1,0);
		InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(InteractiveDeleteAction, idle));
	}

	void on_commit(const EvType&)
	{
		dicmdDeleteObj(m_workingSet, m_position).silent_execute();
		InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(InteractiveDeleteAction, idle));
	}

	void idle(const EvType& ev)
	{
		//if (ev != MC)
		//	return;

		InteractiveCommandBase::set_next_handler(HANDLE_FUNCTION(InteractiveDeleteAction, on_click));
	}

	void on_click(const EvType& ev)
	{
		if (ev == MC)
		{
			m_position = InteractiveCommandBase::get_last_point();
			on_commit(OTHER);
		}
	}
private:
	QPoint m_position;
	WorkingSetPtr m_workingSet = nullptr;
};
#endif
