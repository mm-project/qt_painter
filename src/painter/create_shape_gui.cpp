#include "create_shape_gui.hpp"

#include "controller.hpp"
#include "icons.hpp"

// qt Ribbon 
#include "qribbon.hpp"
#include "qribbongroup.hpp"
#include "qribbonwidget.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include <QLayout>
#include <QSignalMapper>

const QSize globalSize(20, 20);
const QStringList Shapes {"line", "rectangle", "ellipse", "polygon"};

///////////////////////////////////////////////////////////////////////////////
//
// implementation create_shape_gui
//
create_shape_gui::create_shape_gui(QWidget* p)
	: QWidget(p)
{
	QRibbonWidget* ribbonWidget = new QRibbonWidget;
	build_shapes_group(ribbonWidget);
	build_colors(ribbonWidget);

	QRibbon* ribbon = new QRibbon;
	ribbon->addTab(ribbonWidget, "Home");

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(ribbon);

	layout->setSpacing(0);
	layout->setMargin(0);

	setLayout(layout);
}

void create_shape_gui::build_shapes_group(QRibbonWidget* ribbonWidget)
{
	QRibbonGroup* ribbonGroup = new QRibbonGroup;
	ribbonGroup->setTitle("Shapes");

	QGroupBox* bShapes = new QGroupBox(this);

	QHBoxLayout* s_layout = new QHBoxLayout;
	QSignalMapper* mapper = new QSignalMapper(this);
	connect(mapper, SIGNAL(mapped(int)), this, SLOT(createShape(int)));

	for (int i = 0; i < Shapes.size(); ++i)
	{
		QIcon icon(IconDir + Shapes[i] + ".png");
		QPushButton* button = new QPushButton(this);
		button->setIcon(icon);
		button->setFixedSize(globalSize);
		button->setFlat(true);
		s_layout->addWidget(button);
		connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
		mapper->setMapping(button, i);
	}
	bShapes->setLayout(s_layout);
	
	ribbonGroup->addButton(bShapes);
	ribbonWidget->addGroup(ribbonGroup);
}

void create_shape_gui::build_colors(QRibbonWidget* ribbonWidget)
{
	QStringList texts {"white", "yellow", "green", "red", "gray", 
		"blue", "magenta", "cyan"};
	QRibbonGroup* ribbonGroup = new QRibbonGroup;
	ribbonGroup->setTitle("Colors");

	QWidget* widget = new QWidget;

	QGridLayout* layout = new QGridLayout;
	layout->setSpacing(1);
	layout->setMargin(0);
	layout->setContentsMargins(0, 0, 0, 0);
	QSignalMapper* mapper = new QSignalMapper(this);
	connect(mapper, SIGNAL(mapped(const QString&)), this,
		SLOT(pen_color_changed(const QString&)));

	for (int i = 0; i < texts.size(); ++i)
	{
		QIcon icon(IconDir + texts[i] + ".png");
		QPushButton* button = new QPushButton(this);
		button->setObjectName(texts[i]);
		button->setIcon(icon);
		button->setFixedSize(globalSize);
		layout->addWidget(button, i / 5, i % 5);
		connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
		mapper->setMapping(button, texts[i]);
	}

	for (int i = 8; i < 15; ++i)
	{
		QPushButton* button = new QPushButton(this);
		button->setFixedSize(globalSize);
		layout->addWidget(button, i / 5, i % 5);
	}

	widget->setLayout(layout);

	ribbonGroup->addButton(widget);
	ribbonWidget->addGroup(ribbonGroup);
	ribbonWidget->addStretch(20);
}

void create_shape_gui::build_gap_style(QRibbonWidget* ribbonWidget)
{
	QRibbonGroup* ribbonGroup = new QRibbonGroup;
	ribbonGroup->setTitle("Gap Style");
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
                map["white"] = Qt::white;
                map["black"] = Qt::black;
                map["green"] = Qt::green;
                map["red"] = Qt::red;
                map["gray"] = Qt::gray;
                map["blue"] = Qt::blue;
                map["magenta"] = Qt::magenta;
                map["cyan"] = Qt::cyan;
                map["yellow"] = Qt::yellow;
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

void create_shape_gui::pen_color_changed(const QString& s)
{
	controller* c = controller::get_instance();
	c->change_brush_color(get_color_from_string(s));
	emit something_changed();
}

void create_shape_gui::createShape(int i)
{
	switch (i)
	{
	case 0:
		emit createLine();
		break;
	case 1: 
		emit createRect();
		break;
	case 2:
		emit createEllipse();
		break;
	case 3:
		emit createPolygon();
		break;
	}
}
