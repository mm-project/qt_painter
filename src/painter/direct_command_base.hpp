#ifndef direct_command_base_hpp
#define direct_command_base_hpp

#include "icommand.hpp"
#include "command_option.hpp"

class direct_command_base: public icommand
{
    public:
        virtual void add_arg(CommandOption* o ) {
            m_op = o;
        }

    public
        CommandOption* get_arg() {
            return m_op;
        }
    
    private:
        CommandOption* m_op;
};

#endif
