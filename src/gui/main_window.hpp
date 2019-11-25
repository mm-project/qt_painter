#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

///////////////////////////////////////////////////////////////////////////////
//
// Includes
//
// Qt
#include <QMainWindow>

class canvas;
class create_shape_gui;

///////////////////////////////////////////////////////////////////////////////
//
// @class mainWindow 
//
class main_window : public QMainWindow
{
	Q_OBJECT

public:
	main_window(QWidget* = 0 );
	~main_window();
	bool eventFilter(QObject *object, QEvent *event);

private:
	void make_connections();

private:
	canvas* m_canvas;
	create_shape_gui* m_shapes;
};

#endif
