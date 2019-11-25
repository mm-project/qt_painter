#ifndef ICONS_HPP
#define ICONS_HPP

#include <QApplication>
#include <QString>

//define default value, but also give way to override from compiler options.
#ifndef ICONPATH
    #ifdef Q_OS_WIN
        #define ICONPATH "C:\\Users\\elen\\Desktop\\my_projects\\qt_painter\\etc\\icons\\"
    #else
        //(in linux)
        #define ICONPATH qApp->applicationDirPath()+"/../../etc/icons/"
    #endif  
#endif

    
    static QString getIconDir() {
        return ICONPATH;
    };

    
#endif
