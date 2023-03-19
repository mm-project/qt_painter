#ifndef ICONS_HPP
#define ICONS_HPP

#include <QApplication>
#include <QString>

// define default value, but also give way to override from compiler options.
#ifndef ICONPATH
#ifdef Q_OS_WIN
#define ICONPATH qApp->applicationDirPath() + "\\..\\..\\etc\\icons\\"
#else
//(in linux)
#define ICONPATH qApp->applicationDirPath() + "/../../etc/icons/"
#endif
#endif

#ifndef STYLESPATH
#ifdef Q_OS_WIN
#define STYLESPATH qApp->applicationDirPath() + "\\..\\..\\etc\\styles\\"
#else
//(in linux)
#define STYLESPATH qApp->applicationDirPath() + "/../../etc/styles/"
#endif
#endif

static QString getIconDir()
{
    return ICONPATH;
};

static QString getStylesDir()
{
    return STYLESPATH;
};

#endif
