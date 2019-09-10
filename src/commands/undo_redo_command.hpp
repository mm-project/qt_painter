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
class dicmdTransaction : public DirectCommandBase
{
public:
	dicmdTransaction(int count)
	{
		add_option("-steps", new IntCommandOptionValue(count));
	}

	dicmdTransaction()
	{
		add_option("-steps", new IntCommandOptionValue());
	}

	std::string get_name() override
	{
		return "dicmdTransaction" + getTransactionName(T);
	}

	void undo_n_steps(int c) {
        UndoManager& man = UndoManager::getInstance();
        while ( c-- )    
            man.undo();
    }

    void redo_n_steps(int c) {
        UndoManager& man = UndoManager::getInstance();
        while ( c-- )    
            man.redo();
    }

    void execute() override
	{
		int s(I_ARG("-steps"));
		switch (T)
		{
		case Undo:
            undo_n_steps(s);
			break;
		case Redo:
            redo_n_steps(s);
			break;
		}
	}
};

//I don't think needed this :)
/*
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
		command_manager::get_instance()->return_to_idle();
	}

	void execute() override
	{
		on_commit(OTHER);
	}

private:
	void on_commit(const EvType&)
	{
		dicmdTransaction<T>(1).silent_execute();
		abord();
	}
};
*/

#endif
