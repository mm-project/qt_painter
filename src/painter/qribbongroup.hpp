#ifndef Q_RIBBON_GROUP_H
#define Q_RIBBON_GROUP_H

#include <QWidget>

class QHBoxLayout;
class QLabel;
class QGridLayout;

enum class QRibbonButtonSize
{
	size16,
	size24,
	size32, // defualt?
	size40
};

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

	void addButton(QWidget*, const QString&, QRibbonButtonSize);

private:
	QLabel* m_label;
	QGridLayout* m_buttonLayout;
	int m_row;
	int m_column;

};
#endif