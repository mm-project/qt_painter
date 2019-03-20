#ifndef gui_commands_hpp
#define gui_commands_hpp

/*

//;
//QApplication::sendEvent(w, new QMouseEvent(QEvent::MouseButtonPress, b.get(), Qt::LeftButton, 0, 0));

                
//FIXME make singleton
class RealEventExecutor
{
    public:
        void execute_action(DirectGuiCommandBase& b) {
            switch (b.type()) {
                    case CLICK :
                            return "Line";
                    case RECT:
                            return "Rectangle";
                    case ELLIPSE:
                            return "Ellipse";
                    case POLYGON:
                            return "Polygon";
        }
        
        QWidget* find_widget() {
            return ..
            
        }
};


class DirectGuiCommandBase: public DirectCommandBase
{
    RealEventExecutor ee; 
   
    public:
        virtual void execute() {
            ee->execute_gui_action(*this)
        } 
            
}


class dicmdCanvasAddPoint: public DirectGuiCommandBase
{
};

*/

#endif
