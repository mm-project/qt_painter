#ifndef UNDO_REDO_COMMAND_HPP
#define UNDO_REDO_COMMAND_HPP

#include "interactive_command_base.hpp"
#include "command_manager.hpp"
#include "direct_command_base.hpp"
#include "undo_manager.hpp"

enum TransactionAction
{
	Undo,
	Redo
};

std::string getTransactionName(TransactionAction T)
{
	switch (T)
	{
	case Undo:
		return "Undo";
	case Redo:
		return "Redo";
	}
	return "";
}

template <TransactionAction T>
class dicmdUndoRedo : public DirectCommandBase
{
public:
	dicmdUndoRedo(int count)
	{
		add_option("-steps", new IntCommandOptionValue(count));
	}

	dicmdUndoRedo()
	{
		add_option("-steps", new IntCommandOptionValue());
	}

	std::string get_name() override
	{
		return "dicmd" + getTransactionName(T);
	}

	void execute() override
	{
		UndoManager& man = UndoManager::getInstance();
		switch (T)
		{
		case Undo:
			man.undo();
			break;
		case Redo:
			man.redo();
			break;
		}
	}
};

template <TransactionAction T>
class InteractiveUndoRedo : public InteractiveCommandBase
{
public:
	InteractiveUndoRedo() = default;

	std::string get_name() override
	{
		return "incmd" + getTransactionName(T);
	}

	void abord() override
	{
		command_manager::getInstance().return_to_idle();
	}

	void execute() override
	{
		on_commit(OTHER);
	}

private:
	void on_commit(const EvType&)
	{
		dicmdUndoRedo<T>(1).silent_execute();
		abord();
	}
};

#endif
