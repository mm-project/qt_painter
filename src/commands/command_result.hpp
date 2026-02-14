#define PY_SSIZE_T_CLEAN
#pragma push_macro("slots")
#undef slots
#include "Python.h"
#pragma pop_macro("slots")

namespace {
    std::map<int, IShapePtr> global_fixme1;
    std::map<IShapePtr, int> global_fixme2;
    int global_id = 1;
}

namespace 
{
    std::map<std::string, std::string> to_map(const IShapePtr& shape) {
        return { 
                {"object_type", ObjType2String(shape->getType())}, 
                {"object_id", std::to_string(global_fixme2[shape])} 
            };
    }
}

class ICommandResult
{    
    public:
        virtual PyObject* get_python_object() = 0;
        virtual ~ICommandResult() = default;
};

class EmptyResult: public ICommandResult
{
    public:
        virtual PyObject* get_python_object() { return 0; }
};

template<typename T>
struct is_vector : std::false_type {};

template<typename U, typename Alloc>
struct is_vector<std::vector<U, Alloc>> : std::true_type {};

template<typename T>
class CommandResult: public ICommandResult
{
public:
    CommandResult(T t) : value(std::move(t)), py_obj(nullptr) {}

    PyObject* get_python_object() override { 
        return get_data_impl(value, typename is_vector<T>::type{});
    }
    
private:
    // Case 1: plain object with free to_map()
    template<typename U>
    PyObject* get_data_impl(U& val, std::false_type) {
        py_obj = PyDict_New();
        auto m = to_map(val);
        for (auto& [k, v] : m) {
            PyObject* py_key = PyUnicode_FromString(k.c_str());
            PyObject* py_val = PyUnicode_FromString(v.c_str());
            // Insert into dict
            PyDict_SetItem(py_obj, py_key, py_val);

            // DECREF because PyDict_SetItem increments refs
            Py_DECREF(py_key);
            Py_DECREF(py_val);
            ///PyDict_SetItemString(py_obj, k.c_str(), PyLong_FromLong(v));
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


