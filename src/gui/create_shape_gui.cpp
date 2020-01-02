#include "create_shape_gui.hpp"

#include "controller.hpp"
#include "icons.hpp"
#include "utilities.hpp"

// qt Ribbon 
#include "qribbon.hpp"
#include "qribbongroup.hpp"
#include "qribbonwidget.hpp"
#include "qribbonbutton.hpp"

#include "../core/postman.hpp"

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
	build_colors(ribbonWidget);
	//build_gap_style(ribbonWidget);
	//build_join_style(ribbonWidget);
	build_brush_and_fill(ribbonWidget);

	QRibbon* ribbon = new QRibbon(this);
	ribbon->addTab(ribbonWidget, "Home");

	QRibbonWidget* shapes = new QRibbonWidget(this);
	build_shapes_group(shapes);
	ribbon->addTab(shapes, "Shapes");

	QRibbonWidget* tools = new QRibbonWidget(this);
	buildToolButtons(tools);
	ribbon->addTab(tools, "Tools");
	
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
	QRibbonButton* new_b1 = new QRibbonButton(this, "New", getIconDir() + "create.png", false);
	connect(new_b1, SIGNAL(clicked()), this, SIGNAL(reset()));
	QRibbonButton* close_b1 = new QRibbonButton(this, "Close", getIconDir() + "close.png", false);
	connect(close_b1, SIGNAL(clicked()), this, SIGNAL(close()));
	QRibbonButton* save_b2 = new QRibbonButton(this, "Save", getIconDir() + "save.svg", false);
	connect(save_b2, SIGNAL(clicked()), this, SIGNAL(save()));
	QRibbonButton* load_b = new QRibbonButton(this, "Load", getIconDir() + "upload.svg", false);
	connect(load_b, SIGNAL(clicked()), this, SIGNAL(load()));
	group->addRibbonButton(new_b1);
	group->addRibbonButton(save_b2);
	group->addRibbonButton(load_b);
	group->addRibbonButton(close_b1);
	QRibbonButton* undo_b = new QRibbonButton(this, "Undo", getIconDir() + "undo.svg", false);
	connect(undo_b, SIGNAL(clicked()), this, SIGNAL(undo()));
	QRibbonButton* redo_b = new QRibbonButton(this, "Redo", getIconDir() + "redo.svg", false);
	connect(redo_b, SIGNAL(clicked()), this, SIGNAL(redo()));
	group->addRibbonButton(undo_b);
	group->addRibbonButton(redo_b);
	ribbonWidget->addGroup(group);
}

void create_shape_gui::build_selection(QRibbonWidget* ribbonWidget)
{
	QRibbonGroup* group = new QRibbonGroup(this);
	group->setTitle("Select");

	QRibbonButton* new_b = new QRibbonButton(this, "Point", getIconDir() + "mouse.svg");
	connect(new_b, SIGNAL(start()), this, SIGNAL(selectByPoint()));
	connect(new_b, SIGNAL(end()), this, SIGNAL(abord()));
	connect(new_b, SIGNAL(start()), this, SLOT(discard()));
	connect(new_b, SIGNAL(end()), this, SLOT(restore()));
	//new_b->setFlat(true);
	//connect(new_b, SIGNAL(clicked()), this, SIGNAL(reset()));
	QRibbonButton* close_b = new QRibbonButton(this, "Region", getIconDir() + "selection.svg");
	connect(close_b, SIGNAL(start()), this, SIGNAL(selectByRegion()));
	connect(close_b, SIGNAL(end()), this, SIGNAL(abord()));
	connect(close_b, SIGNAL(start()), this, SLOT(discard()));
	connect(close_b, SIGNAL(end()), this, SLOT(restore()));
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
		connect(button, SIGNAL(start()), mapper, SLOT(map()));
		connect(button, SIGNAL(end()), this, SIGNAL(abord()));
		connect(button, SIGNAL(start()), this, SLOT(discard()));
		connect(button, SIGNAL(end()), this, SLOT(restore()));
		mapper->setMapping(button, i);
		ribbonGroup->addRibbonButton(button);
	}

	ribbonWidget->addGroup(ribbonGroup);

	QRibbonGroup* edit = new QRibbonGroup(this);
	edit->setTitle("Edit");

	QRibbonButton* copy = new QRibbonButton(this, "Copy", getIconDir() + QStringLiteral("copy.svg"));
	connect(copy, SIGNAL(start()), this, SIGNAL(copyShape()));
	connect(copy, SIGNAL(end()), this, SIGNAL(abord()));
	connect(copy, SIGNAL(start()), this, SLOT(discard()));
	connect(copy, SIGNAL(end()), this, SLOT(restore()));
	edit->addRibbonButton(copy);

	QRibbonButton* move  = new QRibbonButton(this, "Move", getIconDir() + QStringLiteral("move.svg"));
	connect(move, SIGNAL(start()), this, SIGNAL(moveShape()));
	connect(move, SIGNAL(end()), this, SIGNAL(abord()));
	connect(move, SIGNAL(start()), this, SLOT(discard()));
	connect(move, SIGNAL(end()), this, SLOT(restore()));
	edit->addRibbonButton(move);

	QRibbonButton* delete_b = new QRibbonButton(this, "Delete", getIconDir() + QStringLiteral("delete.svg"));
	connect(delete_b, SIGNAL(start()), this, SIGNAL(deleteShape()));
	connect(delete_b, SIGNAL(end()), this, SIGNAL(abord()));
	connect(delete_b, SIGNAL(start()), this, SLOT(discard()));
	connect(delete_b, SIGNAL(end()), this, SLOT(restore()));
	edit->addRibbonButton(delete_b);
	ribbonWidget->addGroup(edit);
	ribbonWidget->addStretch(10);
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
                //set_object_name_for_logging(button);
		layout->addWidget(button, i / 5, i % 5);
		mapper->setMapping(button, texts[i]);
		connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
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

void create_shape_gui::build_brush_and_fill(QRibbonWidget* ribbonWidget)
{
	QRibbonGroup* ribbonGroup = new QRibbonGroup(this);
	ribbonGroup->setTitle("Brush");
	QSignalMapper* mapper = new QSignalMapper(this);
	connect(mapper, SIGNAL(mapped(const QString&)), this,
		SLOT(change_brush(const QString&)));

	QStringList styles = { "Horizontal", "Vertical", "Cross" };

	for (int i = 0; i < styles.size(); ++i)
	{
		QRadioButton* button = new QRadioButton(this);
		button->setObjectName(styles[i]);
		connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
		mapper->setMapping(button, styles[i]);
		ribbonGroup->addButton(button, styles[i], QRibbonButtonSize::size16);

		if (i == 0)
			button->click();
	}

	ribbonWidget->addGroup(ribbonGroup);

	QRibbonGroup* ribbonGroup1 = new QRibbonGroup(this);
	ribbonGroup1->setTitle("Fill");
	QSignalMapper* mapper1 = new QSignalMapper(this);
	connect(mapper1, SIGNAL(mapped(const QString&)), this,
		SLOT(change_fill(const QString&)));

	QStringList styles1 = { "Solid", "Dash", "Dot" };

	for (int i = 0; i < styles.size(); ++i)
	{
		QRadioButton* button = new QRadioButton(this);
		button->setObjectName(styles1[i]);
		button->setText(styles1[i]);
		connect(button, SIGNAL(clicked()), mapper1, SLOT(map()));
		mapper1->setMapping(button, styles[i]);
		ribbonGroup1->addButton(button, styles1[i], QRibbonButtonSize::size16);

		if (i == 0)
			button->click();
	}

	ribbonWidget->addGroup(ribbonGroup1);
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
	controller& c = controller::getInstance();
	//fixme nagaina
        //(m_pen_button->isChecked())
	c.change_pen_color(get_color_from_string(s));
	c.change_brush_color(get_color_from_string(s));
	notify_controller_change();
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
	controller& c = controller::getInstance();
	c.change_pen_cap_style(get_cap_style_from_string(s));
	notify_controller_change();
}

void create_shape_gui::join_style_changed(const QString& s)
{
	controller& c = controller::getInstance();
	c.change_pen_join_style(get_join_style_from_string(s));
	notify_controller_change();
}

void create_shape_gui::change_brush(const QString& s)
{
	std::map<std::string, Qt::BrushStyle> mm; 
	mm["Horizontal"] = Qt::HorPattern;
	mm["Vertical"] = Qt::VerPattern;
	mm["Cross"] = Qt::CrossPattern;
	controller& c = controller::getInstance();
	c.change_brush_style(mm[s.toStdString()]);
	notify_controller_change();
}

void create_shape_gui::change_fill(const QString& s)
{
	std::map<std::string, Qt::PenStyle> mm;
	mm["Horizontal"] = Qt::SolidLine;
	mm["Vertical"] = Qt::DashLine;
	mm["Cross"] = Qt::DotLine;
	controller& c = controller::getInstance();
	c.change_pen_style(mm[s.toStdString()]);
	notify_controller_change();
}

void create_shape_gui::notify_controller_change()
{
        //NOTIFY2(CONTROLLER_CHANGED);
        LeCallbackData d;
        NOTIFY(CONTROLLER_CHANGED,d);
        emit something_changed();
}

void create_shape_gui::discard()
{
	if (m_active != nullptr)
	{
		// dicard previous command
		m_active->mute(true);
		m_active->click();
		m_active->mute(false);
	}
	m_active = qobject_cast<QRibbonButton*>(sender());
}

void create_shape_gui::restore()
{
	m_active = nullptr;
}

void create_shape_gui::discardAction()
{
	if (m_active != nullptr)
		// dicard previous command
		m_active->click();
	m_active = nullptr;
}

void create_shape_gui::buildToolButtons(QRibbonWidget* widget)
{
	QRibbonGroup* group = new QRibbonGroup(this);
	//group->setTitle("");
	QRibbonButton* btn = new QRibbonButton(this, "Console", getIconDir() + "console.svg");
	//default is shown
	btn->click();
	connect(btn, SIGNAL(start()), this, SIGNAL(showConsole()));
	connect(btn, SIGNAL(end()), this, SIGNAL(hideConsole()));
	group->addRibbonButton(btn);
	widget->addGroup(group);
	widget->addStretch(1);
}
