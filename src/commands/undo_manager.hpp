#ifndef UNDO_MANAGER_HPP
#define UNDO_MANAGER_HPP

#include "direct_command_base.hpp"

#include "../core/service.hpp"

#include <memory>
#include <vector>

//	DirectUndoCommand don't have undo redo
class UndoCommandBase
{
public:
	virtual void undo() = 0;
	virtual void redo() = 0;

};

class UndoManager : public Service<UndoManager>
{
public:
	UndoManager() {}
	void pushCommand(UndoCommandBase*);
	void pushCommand(std::shared_ptr<UndoCommandBase>);
	void undo();
	void redo();
	void shutDown() override;

	static void registerDependencies();

private:
	std::vector<std::shared_ptr<UndoCommandBase>>	m_stack;
	int m_position = -1;
};

#endif // !UNDO_MANAGER_HPP
