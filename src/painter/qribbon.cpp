#include "qribbon.h"

#include <QLayout>

QRibbon::QRibbon(QWidget* p)
	: QTabWidget(p)
{
	setFixedHeight(125);
	//setStyleSheet("QTabWidget::pane  {border : 0 ;}");
}
