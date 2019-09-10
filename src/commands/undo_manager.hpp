#ifndef UNDO_MANAGER_HPP
#define UNDO_MANAGER_HPP

#include "direct_command_base.hpp"

#include <memory>
#include <vector>

//	DirectUndoCommand don't have undo redo
class UndoCommandBase
{
public:
	virtual void undo() = 0;
	virtual void redo() = 0;

};

class UndoManager
{
public:
	static UndoManager& getInstance();

	void pushCommand(UndoCommandBase*);
	void pushCommand(std::shared_ptr<UndoCommandBase>);
	void undo();
	void redo();
	void clear();

private:
	UndoManager() = default;
	UndoManager(const UndoManager&) = delete;
	UndoManager& operator=(const UndoManager&) = delete;
	UndoManager(const UndoManager&&) = delete;
	UndoManager& operator=(const UndoManager&&) = delete;

private:
	static std::unique_ptr<UndoManager> m_instance;
	std::vector<std::shared_ptr<UndoCommandBase>>	m_stack;
	int m_position = -1;
};

#endif // !UNDO_MANAGER_HPP
