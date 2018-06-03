#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP

#include "command.hpp"
#include "runtime_environment.hpp"
#include "working_set.hpp"

#include <string>


class command_manager  {
	
	public:
		
		command_manager(runtime_environment* r, working_set* s):re(r),ws(s) {
				
		}
	
		command_base* get_create_rectangle_command() {
			return ( new create_rectangle(re,ws) );
		}
		
		void register_command(const char* nm, command_base* cmd) {
			
		}
	
		command_base* get_active_command() {
				return m_current_command;
		}
		
	private:
		//std::map<>;
		command_base* m_current_command;
		runtime_environment* re;
		working_set* ws;
		
};

#endif

