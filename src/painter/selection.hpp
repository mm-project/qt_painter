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
        //bool m_h_on;

public:   
        void clear() {
            highlight_on_off(false);
            WorkingSet::clear();
            m_sb->clear();
            //m_sb = 0;
            //delete m_sb;

            //highlight_on_off();
            //m_rt_pools->clear();
            
        }
        
        void set_working_set(IObjectPool* ws) {
            m_ws = ws;
            //m_h_on = false;
        }
        
        void set_sandbox(ObjectPoolSandbox* ops) {
            m_rt_pools = ops;
            m_sb = new ObjectSandbox();
            m_rt_pools->addChildren(std::shared_ptr<ObjectSandbox>(m_sb));
        }
        
        //asenq te
        void find_by_range_and_add_to_selected(const std::pair<QPoint,QPoint>&) {
            if ( m_ws->getObjects().empty() )
                return;

            highlight_on_off(false);

            //asenq te select ( random objects )
            addObject(m_ws->getObjects()[rand()%m_ws->getObjects().size()]);
            
            //asenq te highlight
            highlight_on_off(true);
            //clear();
        }
        
        void highlight_on_off(bool m_h_on) {
            //m_h_on = !m_h_on;
            if (getObjects().empty())
                return;
            
            //std::cout << "highlight: " << m_h_on << std::endl;
            
            //FIXME remember old propoerties by map
            ShapeProperties sp;
            if ( m_h_on )
                sp.pen_color = Qt::red;
            else
                sp.pen_color = Qt::white;
            
            for ( auto it: getObjects() ) {
                it->updateProperties(sp);
                m_sb->addObject(it);
            }
             
            
            
        }
        
};

#endif
