#include "qribbon.hpp"

#include <QLayout>

QRibbon::QRibbon(QWidget* p)
	: QTabWidget(p)
{
	setFixedHeight(120);
}
