#ifndef Q_RIBBON_H
#define Q_RIBBON_H

#include <QTabWidget>

//	class QRibbon
//	represents the ribbon widgets in tab
class QRibbon : public QTabWidget
{
	Q_OBJECT

public:
	//	Constructor
	explicit QRibbon(QWidget* = 0);

	//	Destructor
	virtual ~QRibbon() {}
};

#endif