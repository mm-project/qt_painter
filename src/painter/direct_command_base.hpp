#ifndef direct_command_base_hpp
#define direct_command_base_hpp

#include "icommand_base.hpp"
#include "command_option.hpp"

class DirectCommandBase: public CommandBase
{
    public:
        virtual CommandType get_type() {
            return Directive;
        }
        
        virtual void add_arg(ICommandOption* o ) {
            m_op = o;
        }

    public:
        ICommandOption* get_arg() {
            return m_op;
        }
    
    private:
        ICommandOption* m_op;
};

#endif
