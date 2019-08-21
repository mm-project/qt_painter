#include "create_shape_gui.hpp"

#include "controller.hpp"
#include "icons.hpp"

// qt Ribbon 
#include "qribbon.hpp"
#include "qribbongroup.hpp"
#include "qribbonwidget.hpp"
#include "qribbonbutton.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QGroupBox>
#include <QLayout>
#include <QSignalMapper>
#include <QLabel>
#include <QComboBox>

const QSize globalSize(22, 22);
const QStringList Shapes {"Line", "Rectangle", "Ellipse", "Polygon"};

///////////////////////////////////////////////////////////////////////////////
//
// implementation create_shape_gui
//
create_shape_gui::create_shape_gui(QWidget* p)
	: QWidget(p)
{
	QRibbonWidget* ribbonWidget = new QRibbonWidget(this);
	build_design(ribbonWidget);
	build_selection(ribbonWidget);
	build_shapes_group(ribbonWidget);
	build_colors(ribbonWidget);
	build_gap_style(ribbonWidget);
	build_join_style(ribbonWidget);
	build_brush_and_pen_pattern(ribbonWidget);

	QRibbon* ribbon = new QRibbon(this);
	ribbon->addTab(ribbonWidget, "Home");

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(ribbon);

	layout->setSpacing(0);
	layout->setMargin(0);

	setLayout(layout);
}

void create_shape_gui::build_design(QRibbonWidget* ribbonWidget)
{
	QRibbonGroup* group = new QRibbonGroup(this);
	group->setTitle("Design");
	QRibbonButton* new_b = new QRibbonButton(this, "New", getIconDir() + "create.png");
	connect(new_b, SIGNAL(clicked()), this, SIGNAL(reset()));
	QRibbonButton* close_b = new QRibbonButton(this, "Close", getIconDir() + "close.png");
	connect(close_b, SIGNAL(clicked()), this, SIGNAL(close()));
	group->addRibbonButton(new_b);
	group->addRibbonButton(close_b);
	ribbonWidget->addGroup(group);
}

void create_shape_gui::build_selection(QRibbonWidget* ribbonWidget)
{
	QRibbonGroup* group = new QRibbonGroup(this);
	group->setTitle("Select");

	QRibbonButton* new_b = new QRibbonButton(this, "Point", getIconDir() + "mouse.svg");
	connect(new_b, SIGNAL(clicked()), this, SIGNAL(selectByPoint()));
	//new_b->setFlat(true);
	//connect(new_b, SIGNAL(clicked()), this, SIGNAL(reset()));
	QRibbonButton* close_b = new QRibbonButton(this, "Region", getIconDir() + "selection.svg");
	connect(close_b, SIGNAL(clicked()), this, SIGNAL(selectByRegion()));
	group->addRibbonButton(new_b);
	group->addRibbonButton(close_b);
	ribbonWidget->addGroup(group);
}

void create_shape_gui::build_shapes_group(QRibbonWidget* ribbonWidget)
{
	QRibbonGroup* ribbonGroup = new QRibbonGroup(this);
	ribbonGroup->setTitle("Create");

	QSignalMapper* mapper = new QSignalMapper(this);
	connect(mapper, SIGNAL(mapped(int)), this, SLOT(createShape(int)));

	for (int i = 0; i < Shapes.size(); ++i)
	{
		QRibbonButton* button = new QRibbonButton(this, Shapes[i], getIconDir() + Shapes[i].toLower() + ".svg");
		connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
		mapper->setMapping(button, i);
		ribbonGroup->addRibbonButton(button);
	}

	ribbonWidget->addGroup(ribbonGroup);
}

void create_shape_gui::build_colors(QRibbonWidget* ribbonWidget)
{
	QStringList texts {"white", "yellow", "green", "red", "gray", 
		"blue", "magenta", "cyan"};
	QRibbonGroup* ribbonGroup = new QRibbonGroup(this);
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
		QIcon icon(getIconDir() + texts[i] + ".png");
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
	//m_pen_button = new QRadioButton(this);
	//m_pen_button->click();
	//ribbonGroup->addButton(m_pen_button, "Pen", QRibbonButtonSize::size24);
	//ribbonGroup->addButton(brush, "Brush", QRibbonButtonSize::size24);
	ribbonWidget->addGroup(ribbonGroup);
}

void create_shape_gui::build_gap_style(QRibbonWidget* ribbonWidget)
{
	QRibbonGroup* ribbonGroup = new QRibbonGroup(this);
	ribbonGroup->setTitle("Cap Style");

	QSignalMapper* mapper = new QSignalMapper(this);
	connect(mapper, SIGNAL(mapped(const QString&)), this,
		SLOT(cap_style_changed(const QString&)));

	QStringList styles = { "Square Cap", "Flat Cap", "Round Cap" };

	for (int i = 0; i < styles.size(); ++i)
	{
		QRadioButton* button = new QRadioButton(this);
		connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
		mapper->setMapping(button, styles[i]);
		ribbonGroup->addButton(button, styles[i], QRibbonButtonSize::size16);

		if (i == 0)
			button->click();
	}

	ribbonWidget->addGroup(ribbonGroup);
}

void create_shape_gui::build_brush_and_pen_pattern(QRibbonWidget* ribbonWidget)
{
	QRibbonGroup* ribbonGroup = new QRibbonGroup(this);
	ribbonGroup->setTitle("Brush And Pen");

	QStringList styles = {"Solid Pattern", "Dense1 Pattern", "Dense2 Pattern", "Dense3 Pattern"
	"Dense4 Pattern", "Dense5 Pattern", "Dense6 Pattern", "Dense7 Pattern", "No Brush", "Horizontal Pattern",
	"Vertical Pattern", "Cross Pattern", "BDiag Pattern", "FDiag Pattern", "Diag Cross Pattern" };

	QComboBox* box = new QComboBox(this);
	box->addItems(styles);

	QStringList penStyles = {"Solid Line", "Dash Line", "Dot Line", "Dash Dot Line", "Dash Dot Dot Line",
		"Custom Dash Line", "No Pen"};

	QComboBox* pen = new QComboBox(this);
	pen->addItems(penStyles);

	ribbonGroup->addButton(box);
	ribbonGroup->addButton(pen);
	ribbonWidget->addGroup(ribbonGroup);
	ribbonWidget->addStretch(10000);
}

void create_shape_gui::build_join_style(QRibbonWidget* ribbonWidget)
{
	QRibbonGroup* ribbonGroup = new QRibbonGroup(this);
	ribbonGroup->setTitle("Join Style");

	QSignalMapper* mapper = new QSignalMapper(this);
	connect(mapper, SIGNAL(mapped(const QString&)), this,
		SLOT(join_style_changed(const QString&)));

	QStringList styles = { "Bevel Join", "Miter Join", "Round Join" };

	for (int i = 0; i < styles.size(); ++i)
	{
		QRadioButton* button = new QRadioButton(this);
		connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
		mapper->setMapping(button, styles[i]);
		ribbonGroup->addButton(button, styles[i], QRibbonButtonSize::size16);

		if (i == 0)
			button->click();
	}

	ribbonWidget->addGroup(ribbonGroup);
	ribbonWidget->addStretch(500);
}

namespace {

typedef std::map<QString, QColor> string_to_color;
typedef std::map<QString, Qt::PenCapStyle> string_to_cap_style;
typedef std::map<QString, Qt::PenJoinStyle> string_to_join_style;

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
}

void create_shape_gui::pen_color_changed(const QString& s)
{
	controller* c = controller::get_instance();
	//(m_pen_button->isChecked())
	//	? c->change_pen_color(get_color_from_string(s))
	//	: c->change_brush_color(get_color_from_string(s));
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

void create_shape_gui::cap_style_changed(const QString& s)
{
	controller* c = controller::get_instance();
	c->change_pen_cap_style(get_cap_style_from_string(s));
	emit something_changed();
}

void create_shape_gui::join_style_changed(const QString& s)
{
	controller* c = controller::get_instance();
	c->change_pen_join_style(get_join_style_from_string(s));
	emit something_changed();
}
