#include "qribbon.hpp"

#include <QLayout>

QRibbon::QRibbon(QWidget* p)
	: QTabWidget(p)
{
	setFixedHeight(125);
	//setStyleSheet("QTabWidget::pane  {border : 0 ;}");
}
