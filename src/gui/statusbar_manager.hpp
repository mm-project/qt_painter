#ifndef STATUS_BAR_MANAGER_HPP
#define STATUS_BAR_MANAGER_HPP

#include <memory>

#include <QStatusBar>
#include <QMovie>
#include <QLabel>

//
//	class StatusBarManager : Singletone
//	Manages StatusBar in MainWindow, allows to change status everywhere
//
class StatusBarManager
{
public:
	//
	//	Methods
	//
	static StatusBarManager& getInstance();
	//	Initialize with StatusBar
	void setStatusBar(QStatusBar*);
	//	Clear the content, call from closing application
	void clear();

	void updateStatusBar(const QString&, bool error, bool processing);

	~StatusBarManager();
private:
	//
	//	Private helpers
	// 
	StatusBarManager();

private:
	//
	//	Contents
	//
	static std::unique_ptr<StatusBarManager>	m_pInstance;
	QStatusBar*									m_pStatusBar = nullptr;
	QMovie*										m_pGif = nullptr;
	QLabel*										m_pStatusBarLabel = nullptr;
	QLabel*										m_pGifLabel = nullptr;
};
#endif