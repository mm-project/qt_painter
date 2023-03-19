#include "qribbon.hpp"

#include "icons.hpp"

#include <QFile>
#include <QLayout>

QRibbon::QRibbon(QWidget *p) : QTabWidget(p)
{
    setFixedHeight(127);

    QFile styles(getStylesDir() + "defaults.qss");
    styles.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styles.readAll());
    setStyleSheet(styleSheet);
}
