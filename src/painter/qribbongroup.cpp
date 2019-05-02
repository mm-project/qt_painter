#include "qribbongroup.hpp"
#include "qribbonbutton.hpp"

#include <QFrame>
#include <QLabel>
#include <QHBoxLayout>
#include <QFile>

QRibbonGroup::QRibbonGroup(QWidget* p)
	: QWidget(p), m_row(0), m_column(0)
{
	m_label = new QLabel(this);

	QFrame* separator = new QFrame(this);
	separator->setFrameShape(QFrame::VLine);
	separator->setAutoFillBackground(false);
	separator->setFrameShadow(QFrame::Plain);
	separator->setLineWidth(1);
	separator->setMidLineWidth(0);

	m_buttonLayout = new QGridLayout;

	QVBoxLayout* labelLayout = new QVBoxLayout;
	labelLayout->addLayout(m_buttonLayout);
	labelLayout->addStretch();
	labelLayout->addWidget(m_label, 0, Qt::AlignCenter | Qt::AlignBottom);

	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addLayout(labelLayout);
	mainLayout->addWidget(separator);
	mainLayout->setContentsMargins(2, 2, 2, 2);

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
	if (size == QRibbonButtonSize::size46 || size == QRibbonButtonSize::size32)
	{
		QVBoxLayout* l = new QVBoxLayout;
		(size == QRibbonButtonSize::size32)
			? button->setFixedSize(32, 32)
			: button->setFixedSize(46, 46);
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

void QRibbonGroup::addRibbonButton(QRibbonButton* button, QRibbonButtonSize buttonSize)
{
	switch (buttonSize)
	{
		case QRibbonButtonSize::size46:
		case QRibbonButtonSize::size32:
		{
			QVBoxLayout* l = new QVBoxLayout;
			(buttonSize == QRibbonButtonSize::size32)
				? button->setIconSize(QSize(32, 32))
				: button->setIconSize(QSize(40, 40));
			l->addWidget(button, 0, Qt::AlignCenter);
			if (m_row != 0)
				++m_column;
			m_row = 0;
			m_buttonLayout->addLayout(l, m_row, m_column++, 3, 1);
			break;
		}
		case QRibbonButtonSize::size24:
		{

			button->setIconSize(QSize(24, 24));
			QHBoxLayout* l = new QHBoxLayout;
			l->addWidget(button);
			m_buttonLayout->addLayout(l, m_row, m_column);
			if (m_row < 1)
				++m_row;
			else
			{
				++m_column;
				m_row = 0;
			}
			break;
		}
		case QRibbonButtonSize::size16:
		{
			button->setIconSize(QSize(16, 16));
			QHBoxLayout* l = new QHBoxLayout;
			l->addWidget(button);
			m_buttonLayout->addLayout(l, m_row, m_column);
			if (m_row < 2)
				++m_row;
			else
			{
				++m_column;
				m_row = 0;
			}
			break;
		}
	}
}