#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>

#include <QAbstractButton>
#include <QObject>
#include <QVariant>
#include <QString>
#include <QPalette>
#include <QStringList>
#include <QMap>

// fixme move tho class 
namespace {
    bool is_w_from_desired_list(QWidget* w) {
        return ( dynamic_cast<QAbstractButton*>(w) ); // ||   qobject_cast<QAbsractButton*>(w)
    }
    /*
    void process_unnamed(QWidget* w) {
        std::cout << "Error: Found widget with no texting property or objectName. Devs please do something. Widget's Hier path : " << cn.toStdString() << " in " << pn.toStdString() << "\n";
        QPalette p(QColor::red)
        w->setPallete(p);
    }*/
    
    QString get_hashed_name(QString pn) {
        
        static QMap<QString,QString> map;
        static int id = 100;

        //QStringList list = res.split("/");
        
        if ( ! map.contains(pn) ) {
            //map[pn] = pn.split("/")[0]+"0x"+QString::number(id); 
            map[pn] = "0x"+QString::number(id)+":"; 
            id++;
        }
        
        return map[pn];
    }
    
    void set_object_name_for_logging(QWidget* w, bool needparentinfo = true ) 
    {
        static int i = 1;

        bool isleaf =  w->children().empty();
        QString cn =  w->metaObject()->className();
        QString tn = isleaf?cn+QStringLiteral("_"):"";
        
        QString pn = "";
        if ( needparentinfo ) { 
            QObject* p = w->parent();
            if ( p != nullptr ) {
                pn = pn  + p->objectName() + "/"; 
                //p = p->parent();
            }
            i++;
        }

        QString wn;
        if ( ! w->objectName().isEmpty() )
            wn = w->objectName();
        else {
            QVariant v = w->property("text");
            wn = v.isValid()?v.toString():w->objectName();
            if ( wn.isEmpty() ) {
                if ( isleaf && is_w_from_desired_list(w) ) {
                    std::cout << "Error: Found widget with no texting property or objectName. Devs please do something. Widget's Hier path : " << cn.toStdString() << " in " << pn.toStdString() << "\n";
                    QPalette palette(Qt::red);
                    w->setPalette(palette);
                }
                wn = tn.toLower()+QString::number(i);
            }
        }
                
        if (isleaf && is_w_from_desired_list(w) )
            w->setObjectName(get_hashed_name(pn)+wn);
        else
            w->setObjectName(pn+wn);
    }
}

#endif
