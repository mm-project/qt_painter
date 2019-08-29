#ifndef DELETE_COMMAND_HPP
#define DELETE_COMMAND_HPP

#include "interactive_command_base.hpp"
#include "command_manager.hpp"
#include "../core/rq/RegionQueryService.hpp"
#include "direct_command_base.hpp"

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
		// find object from RQ
		//FIXME need only one init during shape creation
		RegionQuery& rq = RegionQuery::getInstance();
		for (auto obj : m_workingSet->getObjects())
			rq.insertObject(obj);

		QPoint pos = GET_CMD_ARG(PointCommandOptionValue,"-point");
		IShape* shape = rq.getShapeUnderPos(pos);
		rq.clear(); // temp solution for the crash
		if (shape != nullptr)
		{
			m_workingSet->deleteShape(shape);
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

	virtual void handle_mouse_click(int x, int y) override
	{
		InteractiveCommandBase::handle_mouse_click(x, y); // update state
		m_position = QPoint(x, y);
	}

	virtual void abort() override
	{
		command_manager::get_instance()->return_to_idle();
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
