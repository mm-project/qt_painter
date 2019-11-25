#ifndef command_option_hpp
#define command_option_hpp

#include <QPoint>

class ICommandOption {
    public:
        //FIXME again compiler unhappy with = 0
        virtual ~ICommandOption() {}

};  


class PointCommandOption : public ICommandOption 
{
    public:
        PointCommandOption(const QPoint& p):m_x(p.x()),m_y(p.y()) {}
        PointCommandOption(int x, int y):m_x(x),m_y(y) {}
    
        operator QPoint() {return QPoint(m_x,m_y);}
    
    private:
        int m_x;
        int m_y;
    
};


#endif
