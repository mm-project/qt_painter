#ifndef selection_hpp
#define selection_hpp

#include "iobject_pool.hpp"
#include "runtime_environment.hpp"
#include "working_set.hpp"
#include "service.h"

#include <QPoint>

class HighlightSet;

class Selection : public Service<Selection> , public WorkingSet
{
        HighlightSet* m_sel_highlight_set;
        HighlightSet* m_oa_highlight_set;
        
        IObjectPool* m_ws;
        ObjectPoolSandbox* m_rt_pools;
        ObjectSandbox* m_sb;
        //bool m_h_on;

public:
        virtual std::string getName() override;
public:   
        void clear();
        void set_working_set(IObjectPool* ws);
        void set_sandbox(ObjectPoolSandbox* ops);
        void find_and_highlightselect_shapes_from_region(const std::pair<QPoint,QPoint>& point);
        void highlightselect_shape_under_pos(const QPoint& p );
        void highlight_shape_under_pos(const QPoint& p );
        void select_shape_under_pos(const QPoint& p );
          
};


class HighlightSet : public WorkingSet
{
        //WorkingSet* m_select_set;
        IObjectPool* m_ws;
        ObjectPoolSandbox* m_rt_pools;
        ObjectSandbox* m_sb;
        std::string m_name;
        ShapeProperties m_packet;
        //bool m_h_on;

public:
        virtual std::string getName() override;
public:   
        HighlightSet(const std::string& n,const ShapeProperties& p );
        void create_sandbox(ObjectPoolSandbox* ops);
        void highlight_on();
        void highlight_off();
        void clear();

private:
        void highlight_on_off(bool m_h_on);      
};


#endif
