#ifndef selection_hpp
#define selection_hpp

#include "iobject_pool.hpp"
#include "runtime_pool.hpp"
#include "design.hpp"
#include "callback.hpp"
#include "service.hpp"

#include "rq/RegionQueryService.hpp"

#include <QPoint>
#include <QRect>

#include <string>

class HighlightSet;

class Selection : public Service<Selection> , public Design
{
        HighlightSet* m_sel_highlight_set;
        HighlightSet* m_oa_highlight_set;
        HighlightSet* m_qa_highlight_set;
        QRect m_last_region = { 0, 0, 0, 0 };
        ObjectPoolPtr m_ws;
        
        RuntimePoolManagerPtr m_rt_pools;
        RuntimePoolPtr m_sb;
        std::vector<IShapePtr> m_objs;
        //bool m_h_on;
        RegionQuery& rq = RegionQuery::getInstance();

public:
        virtual std::string getName() const noexcept override;
        virtual void clear() noexcept;
        virtual std::vector<IShapePtr> getObjects() const noexcept;
        virtual IShapePtr addObject(IShapePtr);
        virtual void removeObject(IShapePtr) noexcept {}
        virtual void dumpToFile(const std::string& fname) const;

public:   
        void set_working_set(ObjectPoolPtr ws);
        void set_sandbox(RuntimePoolManagerPtr ops);
        void find_and_highlightselect_shapes_from_region(const std::pair<QPoint,QPoint>& point);
        void select_shape_under_pos(const QPoint& p );
        void highlightselect_shape_under_pos(const QPoint& p );
        void highlight_shape_under_pos(const QPoint& p );
//private:
        void on_controller_update(LeCallbackData&);
        void highlight_last_selected_region(bool);
		void temporary_highlight();
};

class HighlightSet : public Design
{
        //WorkingSet* m_select_set;
        ObjectPoolPtr m_ws;
		RuntimePoolManagerPtr m_rt_pools;
        RuntimePoolPtr m_sb;
        std::string m_name;
        ShapeProperties m_packet;
        //bool m_h_on;

public:
        virtual std::string getName() const noexcept override;
public:   
        HighlightSet(const std::string& n,const ShapeProperties& p );
        void create_sandbox(RuntimePoolManagerPtr ops);
        void highlight_on();
        void highlight_off();
        void clear() noexcept;

private:
        void highlight_on_off(bool m_h_on);      
};
/**/

#endif
