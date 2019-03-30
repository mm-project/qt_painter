#ifndef selection_hpp
#define selection_hpp

#include "working_set.hpp"
#include "service.h"

#include <QPoint>

class Selection : public Service<Selection> , public WorkingSet
{
        //WorkingSet* m_select_set;
        IObjectPool* m_ws;
        ObjectPoolSandbox* m_rt_pools;
        ObjectSandbox* m_sb;
        
   
public:   
        void clear() {
            WorkingSet::clear();
            m_sb->clear();
            //m_rt_pools->clear();
            
        }
        
        void set_working_set(IObjectPool* ws) {
            m_ws = ws;
        }
        
        void set_sandbox(ObjectPoolSandbox* ops) {
            m_rt_pools = ops;
            m_sb = new ObjectSandbox();
            m_rt_pools->addChildren(std::shared_ptr<ObjectSandbox>(m_sb));
        }
        
        //asenq te
        void find_by_range_and_add_to_selected(const std::pair<QPoint,QPoint>&) {
            //asenq te select
            //addObject(m_ws->getObjects()[0]);
            
            //asenq te highlight
            ShapeProperties sp;
            sp.brush_color = Qt::red;
            sp.pen_color = Qt::red;
            
            IShape* s = m_ws->getObjects()[rand()%m_ws->getObjects().size()];
            //new ShapeCreator::get_instance()->create(RECT);
            s->updateProperties(sp);
            
            m_sb->addObject(s);
            
            
        }
};

#endif
