#ifndef QRIBBON_BUTTON_HPP
#define QRIBBON_BUTTON_HPP

#include <QToolButton>

class QRibbonButton : public QToolButton
{
	Q_OBJECT

public:
	QRibbonButton(QWidget* p = nullptr, const QString& = "", const QString& = "");
};

#endif