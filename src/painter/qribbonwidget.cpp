#include "qribbonwidget.hpp"
#include "qribbongroup.hpp"

#include <QHBoxLayout>
#include <QSpacerItem>

QRibbonWidget::QRibbonWidget(QWidget* p)
	: QWidget(p)
{
	m_groupLayout = new QHBoxLayout;
	m_groupLayout->setSpacing(0);
	m_groupLayout->setMargin(0);

	setLayout(m_groupLayout);
}

QRibbonWidget::~QRibbonWidget()
{
	m_groups.clear();
}

void QRibbonWidget::addGroup(QRibbonGroup* group)
{
	m_groups.push_back(group);
	m_groupLayout->addWidget(group);
	m_groupLayout->addStretch();
}

void QRibbonWidget::insertGroup(int i, QRibbonGroup* group)
{
	m_groups.insert(i, group);
	m_groupLayout->insertWidget(i, group);
}

void QRibbonWidget::removeGroup(int i)
{
	QWidget* group = m_groups[i];
	m_groups.remove(i);
	m_groupLayout->removeWidget(group);
}

void QRibbonWidget::addStretch(int i)
{
	m_groupLayout->addStretch(i);
}
