#ifndef CONSOLE_GUI_HPP
#define CONSOLR_GUI_HPP

#include "../io/messenger.hpp"

#include <QFrame>
#include <QTextEdit>
#include <QTextBrowser>
#include <QLineEdit>
#include <QTabWidget>
#include <QDockWidget>
#include <QUrl>

#include <string>

///	@class Console
class ConsoleWidget : public QFrame 
{
	Q_OBJECT

public:
	///	@brief Constructor
	ConsoleWidget(QWidget* = nullptr);

public:
	//	Methods
	void appendText(const QString&, LogMsgSeverity, QString);
	
private slots:
	void onCommandEntered();
        void onConsoleLinkClicked(QUrl);

private:
	virtual bool eventFilter(QObject*, QEvent*) override;

private:
	QTextBrowser* m_view = nullptr;
	QLineEdit* m_console = nullptr;
};

///	@class OuputControl
class OutputWidget : public QFrame
{
	Q_OBJECT

public:
	///	@brief Constructor
	OutputWidget(QWidget* = nullptr);

public:
	void appendText(const QString&, LogMsgSeverity, bool);

private:
	QTextEdit* m_view = nullptr;
};

///	@class ConsoleAssistant
class ConsoleAssistant : public QFrame
{
	Q_OBJECT

public:
	///	@brief Constructor
	ConsoleAssistant(QDockWidget&, QWidget* = nullptr);

public:
	//	Methods
	void updateView(LeCallbackData& data);

private slots:
	void updateWindowTitle(int);

private:
	ConsoleWidget* m_console = nullptr;
	OutputWidget* m_output = nullptr;
  OutputWidget* m_log = nullptr;
	QDockWidget& m_base;
	QTabWidget* m_tabs = nullptr;
};

#endif
