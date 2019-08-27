#include "console.hpp"
#include "../core/postman.hpp"
#include "../io/messenger.hpp"
#include "../io/command_interpreter.hpp"

#include <QLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QTabWidget>

ConsoleWidget::ConsoleWidget(QWidget* parent)
	: QFrame(parent)
{
	// viewer part
	m_view = new QTextEdit(this);
	m_view->setTextInteractionFlags(Qt::TextSelectableByMouse);
	//m_view->setMinimumHeight(100);
	m_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	// writable part
	m_console = new QLineEdit(this);
	m_console->setFixedHeight(25);
	m_console->installEventFilter(this);
	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_view);
	layout->addWidget(m_console);
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);
}

void ConsoleWidget::appendText(const QString& text, LogMsgSeverity severity)
{
	switch (severity)
	{
	case err:
		m_view->setTextColor(Qt::red);
		break;
	case warn:
		m_view->setTextColor(Qt::blue);
		break;
	case ok:
		m_view->setTextColor(Qt::black);
		break;
	case test:
		m_view->setTextColor(Qt::black);
		break;
	case cont:
		m_view->setTextColor(Qt::black);
		break;
	case modal:
		m_view->setTextColor(Qt::black);
		break;
	case info:
		m_view->setTextColor(Qt::green);
		break;
	}
	m_view->append(text);
}

void ConsoleWidget::onCommandEntered()
{
	CommandInterp* pCommand = CommandInterp::get_instance();
	pCommand->interpret_from_string(m_console->text().toStdString());
	m_console->clear();
}

bool ConsoleWidget::eventFilter(QObject* obj, QEvent* evn)
{
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(obj);
	if (lineEdit != nullptr)
	{
		if (evn->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvn = static_cast<QKeyEvent*>(evn);
			auto key = keyEvn->key();
			if (key == Qt::Key_Enter || key == Qt::Key_Return)
			{
				onCommandEntered();
				return true;
			}
		}
	}
	return false;
}


OutputWidget::OutputWidget(QWidget* parent) : QFrame(parent)
{
	m_view = new QTextEdit(this);
	m_view->setTextInteractionFlags(Qt::TextSelectableByMouse);
	//m_view->setMinimumHeight(100);
	m_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_view);
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);
}

void OutputWidget::appendText(const QString& text, LogMsgSeverity)
{
	m_view->append(text);
}

ConsoleAssistant::ConsoleAssistant(QDockWidget& b, QWidget* parent) : QFrame(parent), m_base(b)
{
	m_console = new ConsoleWidget(this);
	m_output = new OutputWidget(this);
	m_tabs = new QTabWidget(this);
	m_tabs->addTab(m_console, QStringLiteral("Console"));
	m_tabs->addTab(m_output, QStringLiteral("Output"));
	m_tabs->setTabPosition(QTabWidget::South);
	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(m_tabs);
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);
	m_base.setWindowTitle("Console");

	// Listening for MESSENGER callback in updateView
	REGISTER_CALLBACK(MESSENGER,&ConsoleAssistant::updateView);
	connect(m_tabs, SIGNAL(currentChanged(int)), this, SLOT(updateWindowTitle(int)));
}

void ConsoleAssistant::updateView(LeCallbackData& d)
{
	MessengerCallbackData& data = dynamic_cast<MessengerCallbackData&>(d);
	LogMsgSeverity s = data.get_severity();
	std::string errcode = data.get_errorcode();
	(s != out)
		? m_console->appendText(QString::fromStdString(data.get_message()), s)
		: m_output->appendText(QString::fromStdString(data.get_message()), s);
}

void ConsoleAssistant::updateWindowTitle(int index)
{
	index == 0 ? m_base.setWindowTitle("Console") : m_base.setWindowTitle("Output");
}
