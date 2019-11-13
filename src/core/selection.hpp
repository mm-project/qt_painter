#ifndef selection_hpp
#define selection_hpp

#include "iobject_pool.hpp"
#include "runtime_environment.hpp"
#include "working_set.hpp"
#include "callback.hpp"
#include "service.hpp"

#include "rq/RegionQueryService.hpp"

#include <QPoint>
#include <QRect>

#include <string>

class HighlightSet;

class Selection : public Service<Selection> , public WorkingSet
{
        HighlightSet* m_sel_highlight_set;
        HighlightSet* m_oa_highlight_set;
        HighlightSet* m_qa_highlight_set;
        QRect m_last_region = { 0, 0, 0, 0 };
        
        IObjectPool* m_ws;
        ObjectPoolSandbox* m_rt_pools;
        ObjectSandbox* m_sb;
        std::vector<IShape*> m_objs;
        //bool m_h_on;
        RegionQuery& rq = RegionQuery::getInstance();

public:
        virtual void clear();
        virtual std::vector<IShape*> getObjects();
        virtual IShape* addObject(IShape*);
        virtual void removeObject(IShape*) {}
        virtual std::string getName();
        virtual void dumpToFile(const std::string& fname);

public:   
        void set_working_set(IObjectPool* ws);
        //void set_sandbox(ObjectPoolSandbox* ops);
        void find_and_highlightselect_shapes_from_region(const std::pair<QPoint,QPoint>& point);
        void select_shape_under_pos(const QPoint& p );
        //void highlightselect_shape_under_pos(const QPoint& p );
        //void highlight_shape_under_pos(const QPoint& p );
        //void select_shape_under_pos(const QPoint& p );
//private:
        void on_controller_update(LeCallbackData&);
        void highlight_last_selected_region(bool);          
};

/*
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
*/

#endif
