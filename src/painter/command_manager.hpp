#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

#include "command.hpp"
#include "runtime_environment.hpp"
#include "working_set.hpp"

#include <string>

 
class command_manager  {
	
	public:
		
		command_manager(runtime_environment* r, working_set* s):re(r),ws(s),m_current_command(0),m_idle_command(new idle_command()) {
			
		}
		
		//FIMXE should be called from outside
		void init() {
			//register_command("create_rect",new create_rectangle_command(re,ws));
			//register_command("idle", new idle_command());
			m_rect_command = new create_rectangle_command(re,ws);
			m_current_command = m_idle_command;
		}
	
		//command_base* get_create_rectangle_command() {
		//	return 
		//}
		
		command_base* invoke_command() {
			m_current_command = m_rect_command;
			return m_rect_command;
		}
		
		//command_base* get_command() {
		//	return 0;
		//}
		
		void register_command(const char* nm, command_base* cmd) {
			m_name2command[nm] = cmd;
		}
	
		command_base* get_active_command() {
			return m_current_command;
		}
		
		bool is_idle() {
			return ( m_current_command == m_idle_command );
		}
		
	private:
		//FIXME !!! map with string
		std::map<std::string, command_base*> m_name2command;
		
		command_base* m_current_command;
		command_base* m_idle_command;
		command_base* m_rect_command;
		
		runtime_environment* re;
		working_set* ws;
		//bool m_is_idle;
};

#endif

