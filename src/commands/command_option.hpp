#ifndef command_option_hpp
#define command_option_hpp

#include <QPoint>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cassert>

//FIXME can't use template because everytime we write ICommandOptionValue,
// we will force to write ICommandOptionValue<T> thus user should also be Template.
//template<typename T>
class ICommandOptionValue 
{
    public:
        //virtual T get() = 0;
        virtual std::string to_string() = 0;
        virtual void from_string(const std::string&) = 0;
        //FIXME again compiler unhappy with = 0
        virtual ~ICommandOptionValue() {}

};  

//FIXME need to think more
/*
class CommandOptionValueBase : public CommandOptionValue<void*> {
    
};
*/

/*
template < typename T >
class CommandOptionValueGetter {
    
    ICommandOptionValue* m_i;    
    
    public:
        CommandOptionValueGetter(ICommandOptionValue* i):m_i(i) {}
        
        T get() {
            return (dynamic_cast<T*>(m_i))->get();
        }
};
#define GET_CMD_ARG(S) CommandOptionValueGetter(get_option_val(S)).get()

*/

//FIXME what if dynamic cast is 0?
#define GET_CMD_ARG(T,Op) (dynamic_cast<T*>(get_option_val(Op)))->get()
//#define GET_CMD_ARG1(Op)  (dynamic_cast<decltype get_option_val(Op)>(get_option_val(Op)))->get()

#define StringListCommandOptionValue ListCommandOptionValue<StringCommandOptionValue>
#define PointListCommandOptionValue ListCommandOptionValue<PointCommandOptionValue>

#define PL_ARG(s) GET_CMD_ARG(PointListCommandOptionValue,s)
#define S_ARG(s) GET_CMD_ARG(StringCommandOptionValue,s)
#define I_ARG(s) GET_CMD_ARG(IntCommandOptionValue,s)


class IntCommandOptionValue : public ICommandOptionValue 
{
    int m_data = 0;
    public:
        IntCommandOptionValue() {}
        IntCommandOptionValue(const int& i):m_data(i) {}
        int get() { return m_data; }
        std::string to_string() { return std::to_string(m_data); }
        void from_string(const std::string& s) { m_data = atoi(s.c_str()); }
    
};


class StringCommandOptionValue : public ICommandOptionValue 
{
    std::string m_str = "";
    public:
        StringCommandOptionValue() {}
        StringCommandOptionValue(const std::string& s):m_str(s) {}
        std::string get() { return m_str; }
        std::string to_string() { return get();}
        void from_string(const std::string& s) { m_str = s; }
    
};

namespace{
std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}
}

template <typename T>
class ListCommandOptionValue : public ICommandOptionValue 
{
    public:
        ListCommandOptionValue<T>() {}
        ListCommandOptionValue<T>(const std::vector<T>& v):m_data(v){}
        
        std::vector<T> get() { return m_data; }

        void from_string(const std::string& str) {
            m_data.clear();
            std::string s = str;
            s = s.substr(1, s.size() - 2);
            //std::cout << s << std::endl;
            for ( auto it: split(s,';') ) {
                T t;
                t.from_string(it);
                m_data.push_back(t);
            }
        }
        
        std::string to_string() {
            std::stringstream z;
            
            z << ";{";
            for(auto it:m_data)
                z << it.to_string() << ";";
            
            std::string a = z.str();
            a = a.substr(1, a.size() - 2);
            z.str("");
            
            z << a << "}";
            
            return z.str();
        }
    
    private:
        std::vector<T> m_data;    
};

class PointCommandOptionValue : public ICommandOptionValue //public ICommandOptionValue<QPoint> 
{
    public:
        PointCommandOptionValue() {}
        PointCommandOptionValue(const QPoint& p):m_x(p.x()),m_y(p.y()) {}
        PointCommandOptionValue(int x, int y):m_x(x),m_y(y) {}

        //FIXME check validity
        void from_string(const std::string& str) {
            m_x = std::atoi(str.substr(1,str.find(",")).c_str()); 
            m_y = std::atoi(str.substr(str.find(",")+1,str.find(")")).c_str()); 
        }

        QPoint get() { return QPoint(m_x,m_y); }
        
        std::string to_string() {
            std::stringstream z;
            z << "("<<m_x<<","<<m_y<<")";
            
            return z.str();
        }
    
    private:
        int m_x;
        int m_y;
    
};


#endif
