#include "qribbongroup.hpp"

#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>

QRibbonGroup::QRibbonGroup(QWidget* p)
	: QWidget(p), m_row(0), m_column(0)
{
	m_label = new QLabel(this);
	QFrame* frame = new QFrame(this);
	frame->setFrameShape(QFrame::VLine);
	m_buttonLayout = new QGridLayout;

	QVBoxLayout* labelLayout = new QVBoxLayout;
	labelLayout->addLayout(m_buttonLayout);
	labelLayout->addStretch();
	labelLayout->addWidget(m_label, 0, Qt::AlignCenter | Qt::AlignBottom);

	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addLayout(labelLayout);
	mainLayout->addWidget(frame);
	mainLayout->setContentsMargins(2, 2, 2, 2);
	//mainLayout->addStretch();

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
	++m_column;
}

void QRibbonGroup::addButton(QWidget* button, const QString& label, QRibbonButtonSize size)
{
	if (size == QRibbonButtonSize::size40 || size == QRibbonButtonSize::size32)
	{
		QVBoxLayout* l = new QVBoxLayout;
		(size == QRibbonButtonSize::size32)
			? button->setFixedSize(32, 32)
			: button->setFixedSize(40, 40);
		l->addWidget(button, 0, Qt::AlignCenter);
		l->addWidget(new QLabel(label), 0, Qt::AlignCenter | Qt::AlignBottom);
		if (m_row != 0)
			++m_column;
		m_row = 0;
		m_buttonLayout->addLayout(l, m_row, m_column++, 3, 1);
	}
	if (size == QRibbonButtonSize::size16)
	{
		button->setFixedSize(16, 16);
		QHBoxLayout* l = new QHBoxLayout;
		l->addWidget(button);
		l->addWidget(new QLabel(label));
		m_buttonLayout->addLayout(l, m_row, m_column);
		if (m_row < 2)
			++m_row;
		else
		{
			++m_column;
			m_row = 0;
		}
	}
	if (size == QRibbonButtonSize::size24)
	{
		button->setFixedSize(24, 24);
		QHBoxLayout* l = new QHBoxLayout;
		l->addWidget(button);
		l->addWidget(new QLabel(label));
		m_buttonLayout->addLayout(l, m_row, m_column);
		if (m_row < 1)
			++m_row;
		else
		{
			++m_column;
			m_row = 0;
		}
	}
}