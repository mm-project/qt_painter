#include <iostream>
#include <string>
#include <vector>

// ---------------- PyObject ----------------
class PyObject {
public:
    PyObject(bool is_vector) : is_vector(is_vector) {
        str = is_vector ? "[" : "{";
    }

    void add_dict(const std::string& key, const std::string& val) {
        str += key + ":" + val + ",";
    }

    void add_dict(const std::string& key, long i) {
        str += key + ":" + std::to_string(i) + ",";
    }

    void add_list(const std::string& val) {
        str += val + ",";
    }

    void print() {
        if (!str.empty() && str.back() == ',') str.pop_back();
        std::cout << str << (is_vector ? "]" : "}") << std::endl;
    }

    std::string get_str() {
        std::string out = str;
        if (!out.empty() && out.back() == ',') out.pop_back();
        out += (is_vector ? "]" : "}");
        return out;
    }

private:
    std::string str;
    bool is_vector;
};

// ---------------- ICommandResult ----------------
class ICommandResult {
public:
    virtual PyObject* get_data() = 0;
    virtual ~ICommandResult() = default;
};

// ---------------- IShape ----------------
class IShape {
public:
    virtual void hello() = 0;
    virtual ~IShape() = default;
};

// ---------------- ShapeProperties ----------------
class ShapeProperties {
public:
    int alpha = 8;
    int beta = 10;
    int gamma = 12;
};

// ---------------- Point ----------------
class Point {
public:
    int x = 0;
    int y = 0;
};

// ---------------- Shape ----------------
class Shape : public IShape {
public:
    void hello() override {}
    int a = 0;
    int b = 1;
    ShapeProperties p;
    std::vector<Point> points;  // ✅ vector of Point
};

// ---------------- to_pyobject free functions ----------------
PyObject* to_pyobject(const ShapeProperties& sp) {
    auto* obj = new PyObject(false);
    obj->add_dict("alpha", sp.alpha);
    obj->add_dict("beta", sp.beta);
    obj->add_dict("gamma", sp.gamma);
    return obj;
}

PyObject* to_pyobject(const Point& pt) {
    auto* obj = new PyObject(false);
    obj->add_dict("x", pt.x);
    obj->add_dict("y", pt.y);
    return obj;
}

template<typename T>
PyObject* to_pyobject(const std::vector<T>& vec) {
    auto* arr = new PyObject(true);
    for (auto& elem : vec) {
        arr->add_list(to_pyobject(elem)->get_str());
    }
    return arr;
}

PyObject* to_pyobject(const Shape& s) {
    auto* obj = new PyObject(false);
    obj->add_dict("a", s.a);
    obj->add_dict("b", s.b);
    obj->add_dict("properties", to_pyobject(s.p)->get_str());

    if (!s.points.empty()) {
        obj->add_dict("points", to_pyobject(s.points)->get_str());
    }

    return obj;
}

// ---------------- Generic CommandResult ----------------
template<typename T>
class CommandResult : public ICommandResult {
public:
    CommandResult(T t) : value(std::move(t)) {}
    PyObject* get_data() override {
        return to_pyobject(value);
    }
private:
    T value;
};

// ---------------- Factories ----------------
ICommandResult* do_something3()
{
    Shape obj;
    obj.points.push_back({1, 2});
    obj.points.push_back({3, 4});
    return new CommandResult<Shape>(obj);
}

ICommandResult* do_something4()
{
    Shape s1; s1.points.push_back({5, 6});
    Shape s2; s2.points.push_back({7, 8});
    Shape s3; // empty points
    std::vector<Shape> objs{ s1, s2, s3 };
    return new CommandResult<std::vector<Shape>>(objs);
}

// ---------------- Main ----------------
int main()
{
    do_something3()->get_data()->print();
    // {a:0,b:1,properties:{alpha:8,beta:10,gamma:12},
    //  points:[{x:1,y:2},{x:3,y:4}]}

    do_something4()->get_data()->print();
    // [
    //  {a:0,b:1,properties:{alpha:8,beta:10,gamma:12},points:[{x:5,y:6}]},
    //  {a:0,b:1,properties:{alpha:8,beta:10,gamma:12},points:[{x:7,y:8}]},
    //  {a:0,b:1,properties:{alpha:8,beta:10,gamma:12}}
    // ]
}
