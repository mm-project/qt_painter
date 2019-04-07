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
	void make_connections();

signals:
	void shape_changed();
	void createLine();
	void createRect();
	void createEllipse();
	void createPolygon();

private:
	QPushButton* m_line_button;
	QPushButton* m_rect_button;
	QPushButton* m_ellipse_button;
	QPushButton* m_polygon_button;

	QVBoxLayout* m_layout;
};

#endif
