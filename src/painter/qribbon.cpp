#include "qribbon.hpp"

#include <QLayout>
#include <QFile>

QRibbon::QRibbon(QWidget* p)
	: QTabWidget(p)
{
	setFixedHeight(127);

	QFile styles("C:\\Users\\elen\\Desktop\\my_projects\\qt_painter\\src\\painter\\defaults.qss");
	styles.open(QFile::ReadOnly);
	QString styleSheet = QLatin1String(styles.readAll());
	setStyleSheet(styleSheet);
}