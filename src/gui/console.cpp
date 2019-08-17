#include "console.hpp"
#include "../io/messenger.hpp"
#include "../io/command_interpreter.hpp"

#include <QLayout>

Console::Console(QWidget* parent)
	: QFrame(parent)
{
	// viewer part
	m_view = new QTextEdit(this);
	m_view->setTextInteractionFlags(Qt::TextSelectableByMouse);
	m_view->setFixedHeight(120);
	m_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);


	// writable part
	m_console = new QLineEdit(this);
	m_console->setFixedHeight(25);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_view);
	layout->addWidget(m_console);
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);

	// Initialize messenger and console connection
	Messenger* pMes = Messenger::get_instance();
	pMes->m_console_callback = std::bind(&Console::updateView, this, std::placeholders::_1);

	connect(m_console, SIGNAL(editingFinished()), this, SLOT(onCommandEntered()));
}

void Console::updateView(const std::string& m)
{
	m_view->append(QString::fromStdString(m));
}

void Console::onCommandEntered()
{
	CommandInterp* pCommand = CommandInterp::get_instance();
	pCommand->interpret_from_string(m_console->text().toStdString());
	m_console->clear();
}