#include "undo_manager.hpp"
#include "command_manager.hpp"

void UndoManager::pushCommand(UndoCommandBase* command)
{
	++m_position;
	if (m_stack.size() < m_position)
		m_stack.emplace_back(std::shared_ptr<UndoCommandBase>(command));
	else
		m_stack[m_position] = std::shared_ptr<UndoCommandBase>(command);
}

void UndoManager::pushCommand(std::shared_ptr<UndoCommandBase> command)
{
	++m_position;
	if (m_stack.size() <= m_position)
		m_stack.emplace_back(std::shared_ptr<UndoCommandBase>(command));
	else
		m_stack[m_position] = std::shared_ptr<UndoCommandBase>(command);
}

void UndoManager::undo()
{
	int temp = m_position;
	if (temp <= -1)
		// error can't undo
		return;
	m_position = temp;
	m_stack[m_position]->undo();
	--m_position;
	command_manager::getInstance().get_main_widget()->update();
}

void UndoManager::redo()
{
	int temp = m_position;
	++temp;
	if (temp >= m_stack.size())
		// error can't redo
		return;
	m_position = temp;
	m_stack[m_position]->redo();
	command_manager::getInstance().get_main_widget()->update();
}

void UndoManager::shutDown()
{
	m_stack.clear();
}

void UndoManager::registerDependencies()
{
	addDependency<command_manager>();
}
