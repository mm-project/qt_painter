#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
//#include "icommand_base.hpp"

#include "../core/runtime_pool.hpp"
#include "../core/design.hpp"
#include "../core/callback.hpp"
#include "../core/service.hpp"

// Stl
#include <string>
#include <iostream>

// Qt
#include <QWidget>
 
///////////////////////////////////////////////////////////////////////////////
//
// @class command manager
//
class renderer;
class CommandBase;

class command_manager : public Service<command_manager> 
{
public:
	void set_main_widget(QWidget* w);
	QWidget* get_main_widget();
	void set_main_renderer(renderer* r);
	renderer* get_main_renderer();
	
	
	void init2(RuntimePoolManagerPtr r, ObjectPoolPtr s);
	void init();
	
	void disactivate_active_command();
	void activate_command(CommandBase* cmd);
	void register_command(CommandBase* cmd);
	CommandBase* find_command(const std::string&);
	//CommandBase* get_command();
	//void register_command(const char* nm, CommandBase* cmd) 
	CommandBase* get_active_command();
	bool is_idle(); 
	void return_to_idle(); 
	
	void mouse_dbl_clicked(int x, int y);
	void mouse_clicked(int x, int y);
	void mouse_moved(int x, int y);
	void key_pressed();
	void update();
	
	void on_viewport_changed(LeCallbackData& d);

	
private:
	renderer* m_renderer;
	//FIXME !!! map with string
	std::map<std::string, CommandBase*> m_name2command;
	
	CommandBase* m_current_command = nullptr;
	CommandBase* m_idle_command = nullptr;
	
	RuntimePoolManagerPtr re;
	ObjectPoolPtr ws;
	//bool m_is_idle;
	QWidget* m_main_widget	= nullptr;
	float m_kx = 1;
	float m_ky = 1;
	int m_dx = 0;
	int m_dy = 0;
};






#endif

