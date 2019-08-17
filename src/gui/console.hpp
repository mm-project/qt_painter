#ifndef CONSOLE_GUI_HPP
#define CONSOLR_GUI_HPP

#include <QFrame>
#include <QTextEdit>
#include <QLineEdit>

#include <string>

class Console : public QFrame 
{
	Q_OBJECT

public:
	Console(QWidget* = nullptr);

public:
	//	Methods
	void updateView(const std::string&);
	
private slots:
	void onCommandEntered();

private:
	QTextEdit* m_view = nullptr;
	QLineEdit* m_console = nullptr;
};
#endif