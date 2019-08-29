#ifndef CREATE_SHAPE_GUI_HPP
#define CREATE_SHAPE_GUI_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
// Qt
class QVBoxLayout;
class QRadioButton;
class QPushButton;

#include <QWidget>

class QRibbonWidget;

///////////////////////////////////////////////////////////////////////////////
//
// @class create_shape_gui : Assistant for creating objects 
//
class create_shape_gui : public QWidget
{
	Q_OBJECT

public:
	create_shape_gui(QWidget* = 0);

private:
	void build_design(QRibbonWidget*);
	void build_shapes_group(QRibbonWidget*);
	void build_colors(QRibbonWidget*);
	void build_gap_style(QRibbonWidget*);
	void build_join_style(QRibbonWidget*);
	void build_brush_and_pen_pattern(QRibbonWidget*);
	void build_selection(QRibbonWidget*);

private slots:
	void createShape(int);
	void pen_color_changed(const QString&);
	void cap_style_changed(const QString&);
	void join_style_changed(const QString&);

signals:
	void reset();
	void close();
	void save();
	void load();
	void shape_changed();
	void createLine();
	void createRect();
	void createEllipse();
	void createPolygon();
	void deleteShape();

	void something_changed();
	void selectByRegion();
	void selectByPoint();

};

#endif
