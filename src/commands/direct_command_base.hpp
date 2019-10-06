#ifndef direct_command_base_hpp
#define direct_command_base_hpp

#include "icommand_base.hpp"
#include "command_option.hpp"
#include "undo_manager.hpp"

class DirectCommandBase: public CommandBase
{
    //bool m_is_aborted;
    
    public:
        //DirectCommandBase():m_is_aborted(false) {}
        DirectCommandBase() {}
        //FIXME !!! how about many args?
        DirectCommandBase(const std::string& n, ICommandOptionValue* v ) {
            add_option(n,v);
        }
        
        virtual ~DirectCommandBase() {
            for (std::pair<const std::string,ICommandOptionValue*>& x: m_ops) {
                delete x.second;
                x.second = 0;
            }  
        }
    
    private: 
        bool check_option_exists(const std::string& s ) {
        //std::map::iterator<std::string,ICommandOptionValue*> i;
            return m_ops.find(s) != m_ops.end();
        }
    
    public:
        virtual bool can_undo() { return true; }
        
        virtual bool is_transaction_cmd() {
            return true;
        }    
        
        virtual void silent_execute() {
            bool clean = true;
            try {
                execute();
            } catch (...) {
                clean = false;
            }
            
            if ( clean )
                Messenger::expose_msg(out,get_cmdname_and_stringified_opts(),is_transaction_cmd());
            else
                Messenger::expose_msg(err,"unknown error");
        }
 
        virtual CommandType get_type() {
            return Directive;
        }

        virtual void log() {
            CommandBase::log_impl(get_cmdname_and_stringified_opts());
        }
        
        virtual void abort() {
            //m_is_aborted = true;
        }

    public:
        //ued by replay_log
        virtual CommandBase* set_arg(const std::string& n, const std::string& v) {
            //std::cout << n << " " << v << std::endl;
            if ( ! check_option_exists(n) )
                return 0;
            
            m_ops[n]->from_string(v);
            return this;
        }

        /*
        add_option(CommandOptions().add(key,value).add(key_value)
        void add_option(CommandOptions co) {
            //for co. add_option(co)
            
        }
        */
        
        //used by us
        void add_option(const std::string& n, ICommandOptionValue* v ) {
            //FIXME check if exisitis
            m_ops[n] = v;
        }

        std::string get_cmdname_and_stringified_opts() {
            std::stringstream z;
            z << get_name() << " ";
            for (std::pair<const std::string,ICommandOptionValue*>& x: m_ops) {
                z << x.first << " " << x.second->to_string() << " ";
            }   
            return z.str();
        }
        
        ICommandOptionValue* get_option_val(const std::string& n) {
            //FIXME check exisits
            return m_ops[n];
        }
    
    private:
        std::map<std::string,ICommandOptionValue*> m_ops;
        //ICommandOptionValue* m_op;
        
};



class NonTransactionalDirectCommandBase : public DirectCommandBase
{
    public:
         NonTransactionalDirectCommandBase() {}
         NonTransactionalDirectCommandBase(const std::string& n, ICommandOptionValue* v ):DirectCommandBase(n,v) {}
         virtual bool is_transaction_cmd() {
            return false;
         }
    
};

class TransactionalDirectCommandBase : public DirectCommandBase , public virtual UndoCommandBase 
{
    public:
         virtual bool is_transaction_cmd() {
            return true;
         }
        
        /*
        virtual void silent_execute() {
            log();
            try {
                //pre_execute(); 
                execute();
                
                //post_execute() 
            } catch(...) {
                Messenger::expose_msg(err,"something went wrong with this command");
                std::cout << " Error: Fixme Exception or Error?? " << std::endl;
                return;
            }
            
            UndoManager::getInstance().pushCommand(std::shared_ptr<TransactionalDirectCommandBase>(this));
        }
        */
         virtual bool is_undoable() {
             return true;
         }
    
         virtual bool is_redoable() {
             return true;
         }
};

#endif
