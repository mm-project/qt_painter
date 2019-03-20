#ifndef direct_command_base_hpp
#define direct_command_base_hpp

#include "icommand_base.hpp"
#include "command_option.hpp"

class DirectCommandBase: public CommandBase
{
    //bool m_is_aborted;
    
    public:
        //DirectCommandBase():m_is_aborted(false) {}
        virtual ~DirectCommandBase() {
            for (std::pair<const std::string,ICommandOptionValue*>& x: m_ops) {
                delete x.second;
                x.second = 0;
            }  
        }
    
    public:
        
        virtual CommandType get_type() {
            return Directive;
        }

        virtual void log(const std::string&) {
            std::cout << get_name() << " " << get_stringified_opts() << std::endl;
        }
        
        virtual void abort() {
            //m_is_aborted = true;
        }

    public:
        void add_option(const std::string& n, ICommandOptionValue* v ) {
            //FIXME check if exisitis
            m_ops[n] = v;
        }

        std::string get_stringified_opts() {
            std::stringstream z;
            for (std::pair<const std::string,ICommandOptionValue*>& x: m_ops) {
                z << x.first << " " << x.second->to_string();
            }  
            return z.str();
        }
        
        ICommandOptionValue* get_option_val(std::string& n) {
            //FIXME check exisits
            return m_ops[n];
        }
    
    private:
        std::map<std::string,ICommandOptionValue*> m_ops;
        //ICommandOptionValue* m_op;
};

#endif
