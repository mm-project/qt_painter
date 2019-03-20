#include "pen_brush_gui.hpp"

#include "controller.hpp"

#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QColor>
#include <QIcon>
#include <QRadioButton>
#include <QCoreApplication>
#include <iostream>
#include <map>


pen_brush_gui::pen_brush_gui(QWidget* p)
        : QWidget(p)
{
        init_layout();
        make_connections();
        init_colors();
        init_cap_style();
        init_join_style();
        init_brush_style();
}

void pen_brush_gui::init_layout()
{
        m_pen_layout = new QVBoxLayout;
        m_brush_layout = new QVBoxLayout;

        QGroupBox* pen_group = new QGroupBox("Pen", this);
        QGroupBox* brush_group = new QGroupBox("Brush", this);
        m_pen_color = new QComboBox(this);
        m_pen_width = new QSpinBox(this);
        m_pen_width->setMinimum(1);
        m_brush_color = new QComboBox(this);
        
        m_pen_cap_style = new QComboBox(this);
        m_pen_join_style = new QComboBox(this);
        m_brush_style = new QComboBox(this);

        QLabel* pen_color_label = new QLabel("Pen Color", this);
        QLabel* pen_width_label = new QLabel("Pen Width", this);
        QLabel* brush_color_label = new QLabel("Brush Color", this);
        QLabel* pen_cap_style_label = new QLabel("Pen Cap Style", this);
        QLabel* pen_join_style_label = new QLabel("Pen Join Style", this);
        QLabel* brush_style_label = new QLabel("Brush Style", this);

        QHBoxLayout* l1 = new QHBoxLayout;
        l1->addWidget(pen_color_label);
        l1->addWidget(m_pen_color);
        m_pen_layout->addLayout(l1);

        QHBoxLayout* l2 = new QHBoxLayout;
        l2->addWidget(pen_width_label);
        l2->addWidget(m_pen_width);
        m_pen_layout->addLayout(l2);

        QHBoxLayout* l3  = new QHBoxLayout;
        l3->addWidget(brush_color_label);
        l3->addWidget(m_brush_color);
        m_brush_layout->addLayout(l3);

        QHBoxLayout* l4 = new QHBoxLayout;
        l4->addWidget(pen_cap_style_label);
        l4->addWidget(m_pen_cap_style);
        m_pen_layout->addLayout(l4);

        QHBoxLayout* l5 = new QHBoxLayout;
        l5->addWidget(pen_join_style_label);
        l5->addWidget(m_pen_join_style);
        m_pen_layout->addLayout(l5);

        QGroupBox* pen_style_group = new QGroupBox("Pen Style", this);
        QHBoxLayout* h_l1 = new QHBoxLayout;
        QHBoxLayout* h_l2 = new QHBoxLayout;
        QHBoxLayout* h_l3 = new QHBoxLayout;
        QHBoxLayout* h_l4 = new QHBoxLayout;
        QHBoxLayout* h_l5 = new QHBoxLayout;

        m_solid_line = new QRadioButton("Solid Line", this);
        m_solid_line ->setObjectName("pbguiSolidLine");

        m_dash_line = new QRadioButton("Dash Line", this);
        m_dash_line ->setObjectName("pbguiSolidLine");
        
        m_dot_line = new QRadioButton("Dot Line", this);
        m_dot_line ->setObjectName("pbguiSolidLine");
        
        m_dash_dot_line = new QRadioButton("Dash Dot Line", this);
        m_dash_dot_line ->setObjectName("pbguiSolidLine");
        
        m_dash_dot_dot_line = new QRadioButton("Dash Dot Dot Line", this);
        m_dash_dot_dot_line ->setObjectName("pbguiSolidLine");
        
        static QIcon i1("icons/solidline.jpg");
        static QIcon i2("icons/dash.jpg");
        static QIcon i3("icons/dot.jpg");
        static QIcon i4("icons/dashdot.jpg");
        static QIcon i5("icons/dashdotdot.jpg");
        m_solid_line->setIcon(i1);
        m_dash_line->setIcon(i2);
        m_dot_line->setIcon(i3);
        m_dash_dot_line->setIcon(i4);
        m_dash_dot_dot_line->setIcon(i5);

        h_l1->addWidget(m_solid_line);
        h_l2->addWidget(m_dash_line);
        h_l3->addWidget(m_dot_line);
        h_l4->addWidget(m_dash_dot_line);
        h_l5->addWidget(m_dash_dot_dot_line);

        QVBoxLayout* v_l = new QVBoxLayout;
        v_l->addLayout(h_l1);
        v_l->addLayout(h_l2);
        v_l->addLayout(h_l3);
        v_l->addLayout(h_l4);
        v_l->addLayout(h_l5);
        m_solid_line->click();

        pen_style_group->setLayout(v_l);
        m_pen_layout->addWidget(pen_style_group);

        QHBoxLayout* l6 = new QHBoxLayout;
        l6->addWidget(brush_style_label);
        l6->addWidget(m_brush_style);
        m_brush_layout->addLayout(l6);
        m_pen_layout->addStretch();
        m_brush_layout->addStretch();

        pen_group->setLayout(m_pen_layout);
        brush_group->setLayout(m_brush_layout);
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(pen_group);
        layout->addWidget(brush_group);
        setLayout(layout);
        
        
}

void pen_brush_gui::make_connections()
{
        connect(m_pen_color, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(pen_color_changed(const QString&)));
        connect(m_brush_color, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(brush_color_changed(const QString&)));
        connect(m_pen_width, SIGNAL(valueChanged(int)), this, SLOT(pen_width_changed(int)));
        connect(m_pen_cap_style, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(pen_cap_style_changed(const QString&)));
        connect(m_pen_join_style, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(pen_join_style_changed(const QString&)));
        connect(m_brush_style, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(brush_style_changed(const QString&)));
        connect(m_solid_line, SIGNAL(pressed()), this, SLOT(change_style_to_solid_line()));
        connect(m_dash_line, SIGNAL(pressed()), this, SLOT(change_style_to_dash_line()));
        connect(m_dot_line, SIGNAL(pressed()), this, SLOT(change_style_to_dot_line()));
        connect(m_dash_dot_line, SIGNAL(pressed()), this, SLOT(change_style_to_dash_dot_line()));
        connect(m_dash_dot_dot_line, SIGNAL(pressed()), this, SLOT(change_style_to_dash_dot_dot_line()));
}

void pen_brush_gui::init_colors()
{
        m_pen_color->addItem("White");
        m_pen_color->addItem("Yellow");
        m_pen_color->addItem("Green");
        m_pen_color->addItem("Red");
        m_pen_color->addItem("Gray");
        m_pen_color->addItem("Blue");
        m_pen_color->addItem("Magenta");
        m_pen_color->addItem("Cyan");

        m_brush_color->addItem("Black");
        m_brush_color->addItem("White");
        m_brush_color->addItem("Yellow");
        m_brush_color->addItem("Green");
        m_brush_color->addItem("Red");
        m_brush_color->addItem("Gray");
        m_brush_color->addItem("Blue");
        m_brush_color->addItem("Magenta");
        m_brush_color->addItem("Cyan");
}

void pen_brush_gui::init_cap_style()
{
        m_pen_cap_style->addItem("Square Cap");
        m_pen_cap_style->addItem("Flat Cap");
        m_pen_cap_style->addItem("Round Cap");
}

void pen_brush_gui::init_join_style()
{
        m_pen_join_style->addItem("Bevel Join");
        m_pen_join_style->addItem("Miter Join");
        m_pen_join_style->addItem("Round Join");
}

void pen_brush_gui::init_brush_style()
{
        m_brush_style->addItem("Solid Pattern");
        m_brush_style->addItem("Dense1 Pattern");
        m_brush_style->addItem("Dense2 Pattern");
        m_brush_style->addItem("Dense3 Pattern");
        m_brush_style->addItem("Dense4 Pattern");
        m_brush_style->addItem("Dense5 Pattern");
        m_brush_style->addItem("Dense6 Pattern");
        m_brush_style->addItem("Dense7 Pattern");
        m_brush_style->addItem("No Brush");
        m_brush_style->addItem("Horizontal Pattern");
        m_brush_style->addItem("Vertical Pattern");
        m_brush_style->addItem("Cross Pattern");
        m_brush_style->addItem("BDiag Pattern");
        m_brush_style->addItem("FDiag Pattern");
        m_brush_style->addItem("Diag Cross Pattern");
}

namespace {

typedef std::map<QString, QColor> string_to_color;
typedef std::map<QString, Qt::PenCapStyle> string_to_cap_style;
typedef std::map<QString, Qt::PenJoinStyle> string_to_join_style;
typedef std::map<QString, Qt::BrushStyle> string_to_brush_style;

QColor get_color_from_string(const QString& s)
{
        static string_to_color map;
        if (map.empty()) {
                map["White"] = Qt::white;
                map["Black"] = Qt::black;
                map["Green"] = Qt::green;
                map["Red"] = Qt::red;
                map["Gray"] = Qt::gray;
                map["Blue"] = Qt::blue;
                map["Magenta"] = Qt::magenta;
                map["Cyan"] = Qt::cyan;
                map["Yellow"] = Qt::yellow;
        }
        auto it = map.find(s);
        return (*it).second;
}

Qt::PenCapStyle get_cap_style_from_string(const QString& s)
{
        static string_to_cap_style map;
        if (map.empty()) {
                map["Square Cap"] = Qt::SquareCap;
                map["Flat Cap"] = Qt::FlatCap;
                map["Round Cap"] = Qt::RoundCap;
        }
        auto it = map.find(s);
        return (*it).second;
}

Qt::PenJoinStyle get_join_style_from_string(const QString& s)
{
        static string_to_join_style map;
        if (map.empty()) {
                map["Bevel Join"] = Qt::BevelJoin;
                map["Miter Join"] = Qt::MiterJoin;
                map["Round Join"] = Qt::RoundJoin;
        }
        auto it = map.find(s);
        return (*it).second;
}

Qt::BrushStyle get_brush_style_from_string(const QString& s)
{
        static string_to_brush_style map;
        if (map.empty()) {
                map["Solid Pattern"] = Qt::SolidPattern;
                map["Dense1 Pattern"] = Qt::Dense1Pattern;
                map["Dense2 Pattern"] = Qt::Dense2Pattern;
                map["Dense3 Pattern"] = Qt::Dense3Pattern;
                map["Dense4 Pattern"] = Qt::Dense4Pattern;
                map["Dense5 Pattern"] = Qt::Dense5Pattern;
                map["Dense6 Pattern"] = Qt::Dense6Pattern;
                map["Dense7 Pattern"] = Qt::Dense7Pattern;
                map["No Brush"] = Qt::NoBrush;
                map["Horizontal Pattern"] = Qt::HorPattern;
                map["Vertical Pattern"] = Qt::VerPattern;
                map["Cross Pattern"] = Qt::CrossPattern;
                map["BDiag Pattern"] = Qt::BDiagPattern;
                map["FDiag Pattern"] = Qt::FDiagPattern;
                map["Diag Cross Pattern"] = Qt::DiagCrossPattern;
        }
        auto it = map.find(s);
        return (*it).second;
}
}

void pen_brush_gui::pen_color_changed(const QString& s)
{
        controller* c = controller::get_instance();
        c->change_pen_color(get_color_from_string(s));
        emit something_changed();
}

void pen_brush_gui::brush_color_changed(const QString& s)
{
        controller* c = controller::get_instance();
        c->change_brush_color(get_color_from_string(s));
        emit something_changed();

}

void pen_brush_gui::pen_width_changed(int i)
{
        controller* c = controller::get_instance();
        c->change_pen_width(i);
        emit something_changed();
}

void pen_brush_gui::pen_cap_style_changed(const QString& s)
{
        controller* c = controller::get_instance();
        c->change_pen_cap_style(get_cap_style_from_string(s));
        emit something_changed();
}

void pen_brush_gui::pen_join_style_changed(const QString& s)
{
        controller* c = controller::get_instance();
        c->change_pen_join_style(get_join_style_from_string(s));
        emit something_changed();
}

void pen_brush_gui::change_style_to_solid_line()
{
        controller* c = controller::get_instance();
        c->change_pen_style(Qt::SolidLine);
        emit something_changed();
}

void pen_brush_gui::change_style_to_dash_line()
{
        controller* c = controller::get_instance();
        c->change_pen_style(Qt::DashLine);
        emit something_changed();
}

void pen_brush_gui::change_style_to_dot_line()
{
        controller* c = controller::get_instance();
        c->change_pen_style(Qt::DotLine);
        emit something_changed();
}

void pen_brush_gui::change_style_to_dash_dot_line()
{
        controller* c = controller::get_instance();
        c->change_pen_style(Qt::DashDotLine);
        emit something_changed();
}

void pen_brush_gui::change_style_to_dash_dot_dot_line()
{
        controller* c = controller::get_instance();
        c->change_pen_style(Qt::DashDotDotLine);
        emit something_changed();
}

void pen_brush_gui::brush_style_changed(const QString& s)
{
        controller* c = controller::get_instance();
        c->change_brush_style(get_brush_style_from_string(s));
        emit something_changed();
}
