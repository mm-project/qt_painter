#ifndef postman_h
#define postman_h

#include "callback.hpp"
#include "service.h"

#include <map>
#include <vector>
#include <string>
#include <functional>
#include <typeinfo> 

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)
#define REGISTER_CALLBACK(A,B) LePostman::get_instance()->register_callback(std::string(STR(A))+std::string("-->" )+std::string(STR(B)),A, callBackFun1(std::bind(B,this,std::placeholders::_1)));
#define NOTIFY(A,B) LePostman::get_instance()->notify(std::string(typeid(this).name())+std::string(__FUNCTION__),A,B);

class LePostman : public Service<LePostman>
{
    public:
		friend class Service<LePostman>;

private:
	LePostman() {}
	LePostman(const LePostman&);
	LePostman operator=(const LePostman&);

    public:
        LeCallbackType add_callback_type(const std::string&);
        //LeCallbackType get_callback_type(const std::string&);
        
        LeCallback register_callback(const std::string& n, const LeCallbackType& t, callBackFun1 f);
        LeCallback register_callback(const std::string& n, const LeCallbackType& t, callBackFun2 f);
        void deregister_callback(LeCallbackType& t, int id);

        void notify(const std::string&, const LeCallbackType&);
        void notify(const std::string&, const LeCallbackType&, LeCallbackData&);
        //void notify(const std::string&, const LeCallbackType&, LeCallbackData);
        

    private:
        std::map<LeCallbackType,std::vector<LeCallback>> m_type2vecfun;
        std::map<int,LeCallbackType> m_index2type;
        int m_types_count;
};

#endif
