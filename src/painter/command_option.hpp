#ifndef command_option_hpp
#define command_option_hpp

#include <QPoint>

class ICommandOption {
    public:
        virtual ~ICommandOption() = 0;

};  


class PointCommandOption : public ICommandOption 
{
    public:
        PointCommandOption(int x, int y):m_x(x),m_y(y) {}
    
        operator QPoint() {return QPoint(m_x,m_y);}
    
    private:
        int m_x;
        int m_y;
    
};


#endif
