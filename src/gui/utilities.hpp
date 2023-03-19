#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include "../io/messenger.hpp"

#include <iostream>

#include <QAbstractButton>
#include <QMap>
#include <QObject>
#include <QPalette>
#include <QString>
#include <QStringList>
#include <QTabBar>
#include <QTabWidget>
#include <QVariant>

// fixme move tho class
namespace
{
namespace utilities
{
bool is_w_from_desired_list(QWidget *w)
{
    return (dynamic_cast<QAbstractButton *>(w) || dynamic_cast<QTabWidget *>(w) ||
            dynamic_cast<QTabBar *>(w)); // ||   qobject_cast<QAbsractButton*>(w)
}
/*
void process_unnamed(QWidget* w) {
    std::cout << "Error: Found widget with no texting property or objectName. Devs please do something. Widget's Hier
path : " << cn.toStdString() << " in " << pn.toStdString() << "\n"; QPalette p(QColor::red) w->setPallete(p);
}*/

QString get_hashed_name(QString pn)
{

    static QMap<QString, QString> map;
    static int id = 100;

    // QStringList list = res.split("/");

    if (!map.contains(pn))
    {
        // map[pn] = pn.split("/")[0]+"0x"+QString::number(id);
        map[pn] = "0x" + QString::number(id) + ":";
        id++;
    }

    return map[pn];
}

void set_object_name_for_logging(QWidget *w, bool needparentinfo = true)
{
    static int i = 1;

    bool isleaf = w->children().empty();
    QString cn = w->metaObject()->className();
    QString tn = isleaf ? cn + QStringLiteral("_") : "";

    // getting current widget's parent name
    // by traversing up to the most top parent, and concetating all parents objectnames
    QString pn = "";
    if (needparentinfo)
    {
        QObject *p = w->parent();
        if (p != nullptr)
        {
            pn = pn + p->objectName() + "/";
            // p = p->parent();
        }
        i++;
    }

    QString wn;
    // if widget has a non empty object name, we consider that it's name.
    if (!w->objectName().isEmpty())
        wn = w->objectName();
    // try to get some other info as it's name
    else
    {
        // text property
        if (dynamic_cast<QAbstractButton *>(w))
        {
            QVariant v;
            v = w->property("text");
            wn = v.isValid() ? v.toString() : w->objectName();
        }
        else if (QTabWidget *tb = dynamic_cast<QTabWidget *>(w))
        {
            std::cout << " tab bar " << std::endl;
            wn = tb->tabText(0).isEmpty() ? w->objectName() : tb->tabText(0);
        }
        // if nothing worked,
        // print warning, and assign name as type + i , like QPushButon1, QToolButton2...
        if (wn.isEmpty())
        {
            if (isleaf && is_w_from_desired_list(w))
            {
                // fixme : if only in test-mode !
                std::cout << "Error: Found widget with no texting property or objectName. Devs please do something. "
                             "Widget's Hier path : "
                          << cn.toStdString() << " in " << pn.toStdString() << "\n";
                // Messenger::expose_msg(warn,"No locator for "+cn.toStdString()+" in " +pn.toStdString());

                QPalette palette(Qt::red);
                w->setPalette(palette);
            }
            wn = tn.toLower() + QString::number(i);
        }
    }

    if (isleaf && is_w_from_desired_list(w))
        w->setObjectName(get_hashed_name(pn) + wn); // want a parent name squized-hashed
    else
        w->setObjectName(pn + wn); //
}
} // namespace utilities
} // namespace
#endif
