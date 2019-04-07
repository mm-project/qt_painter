#include "qribbonwidget.h"
#include "qribbongroup.h"

#include <QHBoxLayout>

QRibbonWidget::QRibbonWidget(QWidget* p)
	: QWidget(p)
{
	m_groupLayout = new QHBoxLayout;

	setLayout(m_groupLayout);
}

QRibbonWidget::~QRibbonWidget()
{
	//for (auto i : m_groups)
	//	i->deleteLater();

	m_groups.clear();
}

void QRibbonWidget::addGroup(QRibbonGroup* group)
{
	m_groups.push_back(group);
	m_groupLayout->addWidget(group);
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