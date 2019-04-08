#ifndef Q_RIBBON_GROUP_H
#define Q_RIBBON_GROUP_H

#include <QWidget>

class QHBoxLayout;
class QLabel;

//	@class QRibbonGroup
//	reprents single button group in the widget
class QRibbonGroup : public QWidget
{
	Q_OBJECT

public:
	//	Constructor
	explicit QRibbonGroup(QWidget* = 0);

	//	Destructor
	virtual	~QRibbonGroup() {} 

public:

	//	@brief Sets group title
	void setTitle(const QString& title);

	//	@brief Retutns group title
	QString title() const;

	// @brief Adds button into the group
	void addButton(QWidget*);

private:
	QLabel* m_label;
	QHBoxLayout* m_buttonLayout;
};
#endif