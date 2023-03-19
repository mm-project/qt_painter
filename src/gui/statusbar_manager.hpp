#ifndef STATUS_BAR_MANAGER_HPP
#define STATUS_BAR_MANAGER_HPP

#include "../core/service.hpp"

#include <memory>

#include <QLabel>
#include <QMovie>
#include <QStatusBar>

//
//	class StatusBarManager : Singletone
//	Manages StatusBar in MainWindow, allows to change status everywhere
//
class StatusBarManager : public Service<StatusBarManager>
{
  public:
    //
    //	Methods
    //
    //	Initialize with StatusBar
    void setStatusBar(QStatusBar *);
    //	Clear the content, call from closing application
    void shutDown() override;

    void clear();

    void updateStatusBar(const QString &, bool error, bool processing);

    StatusBarManager();
    virtual ~StatusBarManager();

  private:
    //
    //	Contents
    //
    QStatusBar *m_pStatusBar = nullptr;
    QMovie *m_pGif = nullptr;
    QLabel *m_pStatusBarLabel = nullptr;
    QLabel *m_pGifLabel = nullptr;
};
#endif