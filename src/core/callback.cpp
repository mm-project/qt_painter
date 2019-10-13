#include "callback.hpp"
#include "postman.hpp"

//TODO
/*
#ifdef DEBUG
    #define DEBUG_IF()
    inline void debug(bool expr) {
        if (!expr)
        
    }
#else
    #define DEBUG_IF() if()
#endif
*/

std::string LeCallback::get_name() {
	return m_name;
}

LeCallback::LeCallback(const std::string& n, callBackFun1 f1, int id, const LeCallbackType& t):m_name(n),m_f1(f1),m_f2(NULL),m_internal_id(id),m_type(t),m_is_purged(false) {
}

LeCallback::LeCallback(const std::string& n, callBackFun2 f2, int id, const LeCallbackType& t):m_name(n),m_f1(NULL),m_f2(f2),m_internal_id(id),m_type(t),m_is_purged(false) {
}

void LeCallback::purge() {
    if (!m_is_purged) {
        LePostman::get_instance()->deregister_callback(m_name,m_type,m_internal_id);
        m_is_purged = true;
    }
}

void LeCallback::call() {
    if ( m_f2 != nullptr ) m_f2();
    //assert(0);
}

void LeCallback::call(LeCallbackData& data) {
    if ( m_f1 != nullptr ) m_f1(data);
}

//void LeCallback::call(LeCallbackData data) {
//    if ( m_f1 != NULL ) m_f1(data);
//}
