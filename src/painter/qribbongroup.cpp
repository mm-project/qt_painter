#include "qribbongroup.h"

#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>

QRibbonGroup::QRibbonGroup(QWidget* p)
	: QWidget(p)
{
	m_label = new QLabel(this);
	QFrame* frame = new QFrame(this);
	frame->setFrameShape(QFrame::VLine);
	m_buttonLayout = new QHBoxLayout;

	QVBoxLayout* labelLayout = new QVBoxLayout;
	labelLayout->addLayout(m_buttonLayout);
	labelLayout->addStretch();
	labelLayout->addWidget(m_label, 0, Qt::AlignCenter | Qt::AlignBottom);

	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addLayout(labelLayout);
	mainLayout->addWidget(frame);
	mainLayout->addStretch();

	setLayout(mainLayout);
}

void QRibbonGroup::setTitle(const QString& title)
{
	m_label->setText(title);
}

QString QRibbonGroup::title() const
{
	return m_label->text();
}

void QRibbonGroup::addButton(QWidget* button)
{
	m_buttonLayout->addWidget(button);
}