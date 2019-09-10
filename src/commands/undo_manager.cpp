#include "undo_manager.hpp"
#include "command_manager.hpp"

std::unique_ptr<UndoManager> UndoManager::m_instance = nullptr;

UndoManager& UndoManager::getInstance()
{
	if (m_instance == nullptr)
		m_instance = std::unique_ptr<UndoManager>(new UndoManager);
	return *m_instance;
}

void UndoManager::pushCommand(UndoCommandBase* command)
{
	m_stack.push_back(std::shared_ptr<UndoCommandBase>(command));
	++m_position;
}

void UndoManager::pushCommand(std::shared_ptr<UndoCommandBase> command)
{
	m_stack.push_back(command);
	++m_position;
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
	command_manager::get_instance()->get_main_widget()->update();
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
	command_manager::get_instance()->get_main_widget()->update();
}

void UndoManager::clear()
{
	m_stack.clear();
}
