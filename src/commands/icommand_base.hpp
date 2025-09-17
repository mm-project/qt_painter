#ifndef icommand_base_hpp
#define icommand_base_hpp

#include "command_manager.hpp"

#include "../io/messenger.hpp"

#include <cassert>
#include <iostream>
#include <string>

#define PY_SSIZE_T_CLEAN
#pragma push_macro("slots")
#undef slots
#include "Python.h"
#pragma pop_macro("slots")

enum CommandType
{
    Interactive = 0,
    Directive
};

namespace 
{
    std::map<std::string, long> to_map(const IShapePtr& s) {
        return { {"a", 1}, {"b", 2} };
    }
}

class ICommandResult
{    
    public:
        virtual PyObject* get_python_object() = 0;
        virtual ~ICommandResult() = default;
};

// ---------------- Trait: detect vector ----------------
template<typename T>
struct is_vector : std::false_type {};

template<typename U, typename Alloc>
struct is_vector<std::vector<U, Alloc>> : std::true_type {};

// ---------------- Generic CommandResult ----------------
template<typename T>
class CommandResult: public ICommandResult
{
public:
    CommandResult(T t) : value(std::move(t)), py_obj(nullptr) {}

    PyObject* get_python_object() override { 
        //return 0;
        return get_data_impl(value, typename is_vector<T>::type{});
    }
    
private:
    // Case 1: plain object with free to_map()
    template<typename U>
    PyObject* get_data_impl(U& val, std::false_type) {
        py_obj = PyDict_New();
        auto m = to_map(val);  // free function
        for (auto& [k, v] : m) {
            PyDict_SetItemString(py_obj, k.c_str(), PyLong_FromLong(v));
        }
        return py_obj;
    }

    // Case 2: vector of objects with free to_map()
    template<typename U>
    PyObject* get_data_impl(U& val, std::true_type) {
        py_obj = new PyObject;
        /*
        for (auto& elem : val) {
            auto m = to_map(elem);  // free function
            std::string inner = "{";
            for (auto it = m.begin(); it != m.end(); ++it) {
                inner += it->first + ":" + std::to_string(it->second) + ",";
            }
            if (!inner.empty() && inner.back() == ',') inner.pop_back();
            inner += "}";
            py_obj->add_list(inner);
        }
        */
        return py_obj;
    }

    T value;
    PyObject* py_obj;
};


//CommandResult<int> --> 
//CommandResult<IShapePtr> -->
//a = createObj(....)
//deleteObj -id a.get_data()

class ICommand
{
  public:
    // virtual void pre_execute() = 0;
    virtual ICommandResult* execute() = 0;
    // virtual void post_execute() = 0;
    virtual void abort() = 0;
    virtual void log() = 0;
    virtual std::string get_name() = 0;
    virtual CommandType get_type() = 0;
    // FIXME

    // FIXME
    virtual ICommandResult* execute_and_log() = 0;

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

    virtual ICommandResult* execute_and_log()
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
    virtual void handle_mouse_move(int, int)
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
