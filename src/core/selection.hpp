#ifndef selection_hpp
#define selection_hpp

#include "iobject_pool.hpp"
#include "runtime_pool.hpp"
#include "design.hpp"
#include "service.hpp"

#include <QPoint>
#include <QRect>

class HighlightSet;

class Selection : public Service<Selection> , public Design
{
        HighlightSet* m_sel_highlight_set;
        HighlightSet* m_oa_highlight_set;
        HighlightSet* m_qa_highlight_set;
        QRect m_last_region = { 0, 0, 0, 0 };
        
        IObjectPool* m_ws;
        RuntimePoolManager* m_rt_pools;
        RuntimePool* m_sb;
        //bool m_h_on;

public:
        virtual std::string getName() const noexcept override;
public:   
        void clear() noexcept;
        void set_working_set(IObjectPool* ws);
        void set_sandbox(RuntimePoolManager* ops);
        void find_and_highlightselect_shapes_from_region(const std::pair<QPoint,QPoint>& point);
        void highlightselect_shape_under_pos(const QPoint& p );
        void highlight_shape_under_pos(const QPoint& p );
        void select_shape_under_pos(const QPoint& p );
        void highlight_last_selected_region(bool);
          
};


class HighlightSet : public Design
{
        //WorkingSet* m_select_set;
        IObjectPool* m_ws;
		RuntimePoolManager* m_rt_pools;
        RuntimePool* m_sb;
        std::string m_name;
        ShapeProperties m_packet;
        //bool m_h_on;

public:
        virtual std::string getName() const noexcept override;
public:   
        HighlightSet(const std::string& n,const ShapeProperties& p );
        void create_sandbox(RuntimePoolManager* ops);
        void highlight_on();
        void highlight_off();
        void clear() noexcept;

private:
        void highlight_on_off(bool m_h_on);      
};


#endif
