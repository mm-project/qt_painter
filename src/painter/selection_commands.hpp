#ifndef selection_commands_hpp
#define selection_commands_hpp

#include "selection.hpp"
#include <QPoint>

class incmdSelectShapesByRegion : public InteractiveCommandBase 
{

public:
        virtual std::string get_name() {
                //FIXME keep stringstream for converting int to str
                return "incmdSelectShapesByRegion";
        }
        
        virtual void execute() {
                m_se = Selection::get_instance();
                m_se->clear();
                set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_idle));
        }
        
        virtual void abort() {
                //FIXME now what?
        }

        
//helpers
private:
        std::pair<QPoint,QPoint> m_reg;
        Selection* m_se;
    
//command cycle
public:
        void on_idle(const EvType& ev) {
                if ( ev != MC )
                        return;
                
                m_se->clear();
                m_reg.first = InteractiveCommandBase::get_last_point();
                set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_first_click));
        }
        
        void on_first_click(const EvType& ev) {
                if ( ev != MC ) 
                        return;
                
                m_reg.second = InteractiveCommandBase::get_last_point();
                set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_finish));
        }
        
        void on_finish(const EvType&) {
                //std::cout << "interactive command COMMIT..." << std::endl;
                m_se->find_by_range_and_add_to_selected(m_reg);
                set_next_handler(HANDLE_FUNCTION(incmdSelectShapesByRegion,on_idle));
        }
};


#endif
