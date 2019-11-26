#include "qribbonbutton.hpp"

QRibbonButton::QRibbonButton(QWidget* p, const QString& text, const QString& iconPath, bool checkable)
	: QToolButton(p)
{
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setText(text);
	setIcon(QIcon(iconPath));
	if (checkable)
		connect(this, SIGNAL(clicked()), this, SLOT(process()));
}

void QRibbonButton::process()
{
	if (!m_running)
	{
		m_style = styleSheet();
		m_running = true;
		setStyleSheet("background-color : lightblue;");
		if (!m_mute)
			emit start();
	}
	else
	{
		m_running = false;
		setStyleSheet(m_style);
		if (!m_mute)
			emit end();
	}
}

void QRibbonButton::mute(bool f)
{
	m_mute = f;
}
