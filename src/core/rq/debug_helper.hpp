#ifndef DEBUG_HELPER
#define DEBUG_HELPER
#include "../ishape.hpp"

#include <iostream>
#include <sstream>
#include <vector>

#ifndef DEBUG_RQ
#define DBG_RQ(what, obj)
#else
#define DBG_RQ(what, obj) debug_rq(what, obj);

namespace
{
static std::map<IShape *, int> objmap;
static int objid = 0;
std::string get_points_str(IShape *obj)
{
    if (obj == nullptr)
        return "INVALID-OBJ";

    std::string result;
    std::stringstream strm;
    for (auto point : obj->getPoints())
        strm << "QPoint(" << point.x() << "," << point.y() << "),";

    result = strm.str();
    result = result.substr(0, result.size() - 1);
    return result;
}

std::string get_lvi_format(IShape *obj)
{
    if (obj == nullptr)
        return "INVALID-OBJ";

    std::string result;
    std::stringstream strm;
    strm << "dicmdCreateObjEllipse -brush 1 -color #000000 -fill 9 -points {"; //(574,209);(737,287)} ;
    for (auto point : obj->getPoints())
        strm << "(" << point.x() << "," << point.y() << ");";

    result = strm.str();
    result = result.substr(0, result.size() - 1);
    result = result + "}";
    return result;
}

void debug_rq(const std::string &what, IShape *obj)
{
    if (what == "insert")
        std::cout << "rq.insertObject(create<ellipse>(" + get_points_str(obj) + "));  //" << (++objid) << ", RQ!\n",
            objmap[obj] = objid;
    else
        std::cout << "rq.removeObject(index2shape[" << objmap[obj] << "]);         //RQ!\n";
}
} // namespace
#endif

#endif
