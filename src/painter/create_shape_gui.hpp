#ifndef CREATE_SHAPE_GUI_HPP
#define CREATE_SHAPE_GUI_HPP

class QVBoxLayout;
class QRadioButton;

#include <QWidget>

class create_shape_gui : public QWidget
{
        Q_OBJECT

public:
        create_shape_gui(QWidget* = 0);

private:
        void make_connections();

signals:
        void shape_changed();

private slots:
        void change_to_line();
        void change_to_rect();
        void change_to_ellipse();
        void change_to_polygon();

private:
        QRadioButton* m_line_button;
        QRadioButton* m_rect_button;
        QRadioButton* m_ellipse_button;
        QRadioButton* m_polygon_button;

        QVBoxLayout* m_layout;
};

#endif
