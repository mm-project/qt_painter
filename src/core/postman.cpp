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
    m_type2vecfun[t].push_back(LeCallback(n,f,m_type2vecfun[t].size()+1,t));
    return m_type2vecfun[t].back();
}

LeCallback LePostman::register_callback(const std::string& n, const LeCallbackType& t,callBackFun2 f) {
    m_type2vecfun[t].push_back(LeCallback(n,f,m_type2vecfun[t].size()+1,t));
    return m_type2vecfun[t].back();
}

void LePostman::deregister_callback(LeCallbackType& t, int id) {
    m_type2vecfun[t].erase(m_type2vecfun[t].begin()+id); 
}

void LePostman::notify(const LeCallbackType& t) {
    //std::cout << "notify1" << std::endl;
    for( auto it : m_type2vecfun[t] ) {
        std::cout << it.get_name() << std::endl;
		it.call();
    }
}

void LePostman::notify(const LeCallbackType& t,LeCallbackData& data) {
    //std::cout << "notify2" << std::endl;
    for( auto it : m_type2vecfun[t] ) {
        std::cout << it.get_name() << std::endl;
        it.call(data);
    }
}