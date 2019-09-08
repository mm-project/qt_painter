#include "postman.hpp"
#include "callback.hpp"

#include <iostream>
#include <cassert>
#include <string>


//LePostman::LePostman() {
//    m_types_count = 0;
//}

LeCallbackType LePostman::add_callback_type(const std::string& name) {
    //LeCallbackType cbt(name);
    LeCallbackType cbt;    
    m_index2type[++m_types_count] = cbt;
    return cbt;
}

/*
LeCallbackType LePostman::get_callbacktype_byname(const std::string&) {
        
}
*/

LeCallback LePostman::register_callback(const std::string& n, const LeCallbackType& t,callBackFun1 f) {
    std::cout << "1will reg callback " << n << " with id" << m_type2vecfun[t].size()+1 << std::endl;

    m_type2vecfun[t].push_back(LeCallback(n,f,m_type2vecfun[t].size()+1,t));
    return m_type2vecfun[t].back();
}

LeCallback LePostman::register_callback(const std::string& n, const LeCallbackType& t,callBackFun2 f) {
    std::cout << "2will reg callback " << n << " with id" << m_type2vecfun[t].size()+1 << std::endl;

    m_type2vecfun[t].push_back(LeCallback(n,f,m_type2vecfun[t].size()+1,t));
    return m_type2vecfun[t].back();
}

void LePostman::deregister_callback(const std::string& n, LeCallbackType& t, int id) {
    std::cout << "--deleting callback " << n << " with id" << id << std::endl;

    m_type2vecfun[t].erase(m_type2vecfun[t].begin()+id); 
}

void LePostman::notify(const std::string& n, const LeCallbackType& t) {
    //std::cout << "notify1" << std::endl;
    for( auto it : m_type2vecfun[t] ) {
        //std::cout << n << " " << it.get_name() << std::endl;
        it.call();
    }
}

//void LePostman::notify(const std::string& n, const LeCallbackType& t,LeCallbackData data) {
//        notify(n,t,data)
//}

void LePostman::notify(const std::string& n, const LeCallbackType& t, LeCallbackData& data) {
    //std::cout << "notify2" << std::endl;
    for( auto it : m_type2vecfun[t] ) {
        std::cout << n << " function is calling " << it.get_name() << " via  ENUM:" << t << std::endl;
        //std::cout << n << " called to " << " via " << std::endl;
        it.call(data);
    }
}
