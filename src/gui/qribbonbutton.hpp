#ifndef QRIBBON_BUTTON_HPP
#define QRIBBON_BUTTON_HPP

#include <QToolButton>

class QRibbonButton : public QToolButton
{
	Q_OBJECT

public:
	QRibbonButton(QWidget* p = nullptr, const QString& = "", const QString& = "", bool = true);

signals:
	void start();
	void end();

private slots:
	void process();


private:
	bool m_running = false;
	QString m_style;
};

#endif