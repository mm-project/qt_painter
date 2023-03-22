#ifndef icommand_base_hpp
#define icommand_base_hpp

#include "command_manager.hpp"

#include "../io/messenger.hpp"

#include <cassert>
#include <iostream>
#include <string>

enum CommandType
{
    Interactive = 0,
    Directive
};

class ICommand
{
  public:
    // virtual void pre_execute() = 0;
    virtual void execute() = 0;
    // virtual void post_execute() = 0;
    virtual void abort() = 0;
    virtual void log() = 0;
    virtual std::string get_name() = 0;
    virtual CommandType get_type() = 0;
    // FIXME

    // FIXME
    virtual void execute_and_log() = 0;

    // FIXME bug, pure virtual dtor makes compiler sad:/
    virtual ~ICommand()
    {
    }
};

class CommandBase : public ICommand
{
    // command_manager* m_cm;

    bool m_is_completed = true;

  public:
    CommandBase()
    {
        // m_cm = command_manager::get_instance();
    }

    virtual void execute_and_log()
    {
        log();
        try
        {
            // pre_execute();
            execute();
            // post_execute()
        }
        catch (...)
        {
            Messenger::expose_msg(err, "something went wrong with this command -> " + get_name());
            std::cout << " Error: Fixme Exception or Error?? " << std::endl;
        }
    }

    virtual bool is_completed()
    {
        return m_is_completed;
    }

    void set_can_complete(bool b)
    {
        m_is_completed = b;
    }

    virtual bool is_transaction_cmd()
    {
        return false;
    }
    // by default commands log just their name,
    // for anothers like directive commands, they can redifne what they need to do
    virtual void log()
    {
        log_impl(get_name());
    }

    // virtual void activate() {}
    virtual CommandBase *set_arg(const std::string &, const std::string &)
    {
        return nullptr;
    }

    // FIXME should not be here !!!
    virtual void handle_mouse_click(int, int)
    {
    }
    virtual void handle_mouse_dblclick(int, int)
    {
    }
    virtual void handle_mouse_release(int, int)
    {
    }
    virtual void handle_mouse_move(int, int, bool)
    {
    }
    virtual void handle_mouse_press(int, int)
    {
    }
    virtual void handle_key_press()
    {
    }
    virtual void handle_update()
    {
    }

    // CommandManager* cm() {
    //     return m_cm;
    // }

    // standart implementation
    void log_impl(const std::string &str)
    {
        Messenger::log_command(str, is_transaction_cmd());
    }
};

#endif
