#include "qribbonbutton.hpp"

QRibbonButton::QRibbonButton(QWidget* p, const QString& text, const QString& iconPath)
	: QToolButton(p)
{
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setText(text);
	setIcon(QIcon(iconPath));
}