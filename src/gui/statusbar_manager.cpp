#include "statusbar_manager.hpp"

#include "icons.hpp"

std::unique_ptr<StatusBarManager> StatusBarManager::m_pInstance = nullptr;

StatusBarManager& StatusBarManager::getInstance()
{
	if (m_pInstance == nullptr)
		m_pInstance = std::unique_ptr<StatusBarManager>(new StatusBarManager);
	return *m_pInstance;
}

void StatusBarManager::setStatusBar(QStatusBar* pBar)
{
	m_pStatusBar = pBar;
	m_pStatusBar->addWidget(m_pStatusBarLabel);
	m_pStatusBar->addWidget(m_pGifLabel);
}

void StatusBarManager::clear()
{
	m_pStatusBarLabel->setText("");
}

void StatusBarManager::removeStatusBar()
{
	m_pStatusBar = nullptr;
	m_pGif->stop();
	m_pGifLabel->setHidden(true);
}

StatusBarManager::StatusBarManager()
{
	m_pGif = new QMovie(getIconDir() + "process.gif");
	m_pGif->setScaledSize(QSize(20, 20));
	m_pStatusBarLabel = new QLabel("StatusBar");
	m_pGifLabel = new QLabel();
	m_pGifLabel->setMovie(m_pGif);
	m_pGifLabel->setHidden(true);
}

StatusBarManager::~StatusBarManager()
{
	//delete m_pStatusBarLabel;
	//delete m_pGifLabel;
	//delete m_pGif;
}

void StatusBarManager::updateStatusBar(const QString& text, bool error, bool processing)
{
	!error ? m_pStatusBar->setStyleSheet("color: red")
		: m_pStatusBar->setStyleSheet("color: black");
	m_pStatusBarLabel->setText(text);

	if (processing)
	{
		m_pGif->start();
		m_pGifLabel->setHidden(false);
	}
	else
	{
		m_pGif->stop();
		m_pGifLabel->setHidden(true);
	}
}
