#ifndef DEBUG_HELPER
#define DEBUG_HELPER
#include "../ishape.hpp"

#include <iostream>
#include <vector>
#include <sstream>

#ifdef RQ_DUMP_TEST_INFO
    #define DBG_RQ(what,obj)
#else
    #define DBG_RQ(what,obj) debug_rq(what,obj);

    namespace{ 
    static std::map<IShape**,int> map;
    static int objid = 0;
    std::string get_points_str(IShape** obj) 
    {
        if ( obj == nullptr ) 
            return "INVALID-OBJ";
        
        std::string result;
        std::stringstream strm;
        for (auto point: (*obj)->getPoints() )
            strm  << "QPoint(" << point.x() << "," << point.y() << "),";
        
        result = strm.str();
        result = result.substr(0, result.size()-1);
        return result;
    }
    
    void debug_rq(const std::string& what, IShape** obj) 
    {
        if ( what == "insert")
            std::cout << "rq.insertObject(create<ellipse>("+get_points_str(obj)+"));  //"<< (objid) << ", RQ!\n",    map[obj]=objid++;
        else
            std::cout << "rq.removeObject(index2shape["<< map[obj] << "]);         //RQ!\n";        
    }
    }
#endif    

#endif
