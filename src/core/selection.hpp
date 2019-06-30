#ifndef selection_hpp
#define selection_hpp

#include "iobject_pool.hpp"
#include "runtime_environment.hpp"
#include "working_set.hpp"
#include "service.h"

#include <QPoint>

class Selection : public Service<Selection> , public WorkingSet
{
        //WorkingSet* m_select_set;
        IObjectPool* m_ws;
        ObjectPoolSandbox* m_rt_pools;
        ObjectSandbox* m_sb;
        //bool m_h_on;

public:   
        void clear();
        void set_working_set(IObjectPool* ws);
        void set_sandbox(ObjectPoolSandbox* ops);
        void find_by_range_and_add_to_selected(const std::pair<QPoint,QPoint>& point);
        void highlight_on_off(bool m_h_on);        
};

#endif
