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
	void build_brush_and_fill(QRibbonWidget*);
	void build_selection(QRibbonWidget*);

private slots:
	void createShape(int);
	void pen_color_changed(const QString&);
	void cap_style_changed(const QString&);
	void join_style_changed(const QString&);
	void change_fill(const QString&);
	void change_brush(const QString&);

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

	void something_changed();
	void selectByRegion();
	void selectByPoint();

private:
	QPushButton* m_line_button = nullptr;
	QPushButton* m_rect_button = nullptr;
	QPushButton* m_ellipse_button = nullptr;
	QPushButton* m_polygon_button = nullptr;
	//QRadioButton* m_pen_button = nullptr;
};

#endif
