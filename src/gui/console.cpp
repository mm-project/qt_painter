#include "console.hpp"
#include <QTextEdit>
#include <QLayout>

Console::Console(QWidget* parent)
	: QFrame(parent)
{
	// viewer part
	QTextEdit* m_view = new QTextEdit(this);
	m_view->setTextInteractionFlags(Qt::TextSelectableByMouse);
	m_view->setFixedHeight(120);
	m_view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);


	// writable part
	QTextEdit* m_console = new QTextEdit(this);
	m_console->setFixedHeight(25);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_view);
	layout->addWidget(m_console);
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);
}