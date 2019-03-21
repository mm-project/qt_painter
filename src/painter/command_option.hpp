#ifndef command_option_hpp
#define command_option_hpp

#include <QPoint>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cassert>

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

/*
class CommandOptionValueBase : public CommandOptionValue<void*> {
    
};
*/

class StringCommandOptionValue : public ICommandOptionValue 
{
    std::string m_str;
    public:
        StringCommandOptionValue(const std::string& s):m_str(s) {}
        std::string get() { return m_str; }
        std::string to_string() { return get();}
        void from_string(const std::string& s) { m_str = s; }
    
};

class PointCommandOptionValue : public ICommandOptionValue //public ICommandOptionValue<QPoint> 
{
    public:
        PointCommandOptionValue(const QPoint& p):m_x(p.x()),m_y(p.y()) {}
        PointCommandOptionValue(int x, int y):m_x(x),m_y(y) {}

        void from_string(const std::string& str) {
            //m_x = std::atoi(str.substr(0,1).c_str()); 
            //m_y = std::atoi(str.substr(3,4).c_str()); 
            //m_x = std::atoi(str.substr(0,3).c_str()); 
            //m_y = std::atoi(str.substr(str.find(","),str.find(")")).c_str()); 
            //std::cout << m_x << " " << m_y << std::endl;
            
            m_x = rand()%800;
            m_y = rand()%800;
            //assert(0);
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
