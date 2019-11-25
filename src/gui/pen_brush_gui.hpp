#ifndef PEN_BRUSH_GUI_HPP
#define PEN_BRUSH_GUI_HPP

#include <QWidget>

class QComboBox;
class QSpinBox;
class QVBoxLayout;
class QRadioButton;

class pen_brush_gui : public QWidget
{
        Q_OBJECT

public:
        pen_brush_gui(QWidget* = 0);
        //bool eventFilter(QObject *object, QEvent *event);

private:
        void init_layout();
        void make_connections();
        void init_colors();
        void init_cap_style();
        void init_join_style();
        void init_brush_style();

signals:
        void something_changed();
        
private slots:
        void pen_color_changed(const QString&);
        void brush_color_changed(const QString&);
        void brush_style_changed(const QString&);
        void pen_width_changed(int);
        void pen_cap_style_changed(const QString&);
        void pen_join_style_changed(const QString&);
        void change_style_to_solid_line();
        void change_style_to_dash_line();
        void change_style_to_dot_line();
        void change_style_to_dash_dot_line();
        void change_style_to_dash_dot_dot_line();

private:
        QVBoxLayout* m_pen_layout = nullptr;
        QVBoxLayout* m_brush_layout = nullptr;

        // pen
        QComboBox* m_pen_color = nullptr;
        QSpinBox* m_pen_width = nullptr;
        //pen styles
        QRadioButton* m_solid_line = nullptr;
        QRadioButton* m_dash_line = nullptr;
        QRadioButton* m_dot_line = nullptr;
        QRadioButton* m_dash_dot_line = nullptr;
        QRadioButton* m_dash_dot_dot_line = nullptr;

        QComboBox* m_pen_cap_style = nullptr;
        QComboBox* m_pen_join_style = nullptr;

        // color
        QComboBox* m_brush_color = nullptr;
        QComboBox* m_brush_style = nullptr;
};

#endif
