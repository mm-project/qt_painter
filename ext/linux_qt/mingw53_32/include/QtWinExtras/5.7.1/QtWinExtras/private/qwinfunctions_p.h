/****************************************************************************
 **
 ** Copyright (C) 2016 Ivan Vizir <define-true-false@yandex.com>
 ** Contact: https://www.qt.io/licensing/
 **
 ** This file is part of the QtWinExtras module of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial License Usage
 ** Licensees holding valid commercial Qt licenses may use this file in
 ** accordance with the commercial license agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and The Qt Company. For licensing terms
 ** and conditions see https://www.qt.io/terms-conditions. For further
 ** information use the contact form at https://www.qt.io/contact-us.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 3 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL3 included in the
 ** packaging of this file. Please review the following information to
 ** ensure the GNU Lesser General Public License version 3 requirements
 ** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 2.0 or (at your option) the GNU General
 ** Public license version 3 or any later version approved by the KDE Free
 ** Qt Foundation. The licenses are as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
 ** included in the packaging of this file. Please review the following
 ** information to ensure the GNU General Public License requirements will
 ** be met: https://www.gnu.org/licenses/gpl-2.0.html and
 ** https://www.gnu.org/licenses/gpl-3.0.html.
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#ifndef QWINFUNCTIONS_P_H
#define QWINFUNCTIONS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QString>
#include <qt_windows.h>
#include <uxtheme.h>

QT_BEGIN_NAMESPACE

enum qt_DWMWINDOWATTRIBUTE
{
    qt_DWMWA_NCRENDERING_ENABLED = 1,
    qt_DWMWA_NCRENDERING_POLICY,
    qt_DWMWA_TRANSITIONS_FORCEDISABLED,
    qt_DWMWA_ALLOW_NCPAINT,
    qt_DWMWA_CAPTION_BUTTON_BOUNDS,
    qt_DWMWA_NONCLIENT_RTL_LAYOUT,
    qt_DWMWA_FORCE_ICONIC_REPRESENTATION,
    qt_DWMWA_FLIP3D_POLICY,
    qt_DWMWA_EXTENDED_FRAME_BOUNDS,
    qt_DWMWA_HAS_ICONIC_BITMAP,
    qt_DWMWA_DISALLOW_PEEK,
    qt_DWMWA_EXCLUDED_FROM_PEEK,
    qt_DWMWA_CLOAK,
    qt_DWMWA_CLOAKED,
    qt_DWMWA_FREEZE_REPRESENTATION,
    qt_DWMWA_LAST
};

enum qt_DWMFLIP3DWINDOWPOLICY {
    qt_DWMFLIP3D_DEFAULT,
    qt_DWMFLIP3D_EXCLUDEBELOW,
    qt_DWMFLIP3D_EXCLUDEABOVE,
    qt_DWMFLIP3D_LAST
};

#include <pshpack1.h>

struct qt_DWM_BLURBEHIND {
    DWORD dwFlags;
    BOOL  fEnable;
    HRGN  hRgnBlur;
    BOOL  fTransitionOnMaximized;
};

#include <poppack.h>

const int qt_DWM_BB_ENABLE                = 0x00000001;
const int qt_DWM_BB_BLURREGION            = 0x00000002;
const int qt_DWM_BB_TRANSITIONONMAXIMIZED = 0x00000004;

struct QtDwmApiDll
{
    typedef HRESULT (STDAPICALLTYPE *DwmGetColorizationColor)(DWORD *, BOOL *);
    typedef HRESULT (STDAPICALLTYPE *DwmSetWindowAttribute)(HWND, DWORD, LPCVOID, DWORD);
    typedef HRESULT (STDAPICALLTYPE *DwmGetWindowAttribute)(HWND, DWORD, PVOID, DWORD);
    typedef HRESULT (STDAPICALLTYPE *DwmExtendFrameIntoClientArea)(HWND, const MARGINS *);
    typedef HRESULT (STDAPICALLTYPE *DwmEnableBlurBehindWindow)(HWND, const qt_DWM_BLURBEHIND *);
    typedef HRESULT (STDAPICALLTYPE *DwmIsCompositionEnabled)(BOOL *);
    typedef HRESULT (STDAPICALLTYPE *DwmEnableComposition)(UINT);
    typedef HRESULT (STDAPICALLTYPE *DwmSetIconicThumbnail)(HWND, HBITMAP, DWORD);
    typedef HRESULT (STDAPICALLTYPE *DwmSetIconicLivePreviewBitmap)(HWND, HBITMAP, POINT *, DWORD);
    typedef HRESULT (STDAPICALLTYPE *DwmInvalidateIconicBitmaps)(HWND);

    QtDwmApiDll()
        : dwmGetColorizationColor(0), dwmSetWindowAttribute(0), dwmGetWindowAttribute(0)
        , dwmExtendFrameIntoClientArea(0), dwmEnableBlurBehindWindow(0)
        , dwmIsCompositionEnabled(0), dwmEnableComposition(0)
        , dwmSetIconicThumbnail(0), dwmSetIconicLivePreviewBitmap(0), dwmInvalidateIconicBitmaps(0)
    {}

    void init()
    {
        if (!dwmSetWindowAttribute && QSysInfo::windowsVersion() >= QSysInfo::WV_VISTA)
            resolve();
    }

    void resolve();

    template <class T> static T windowAttribute(HWND hwnd, DWORD attribute, T defaultValue);
    template <class T> static void setWindowAttribute(HWND hwnd, DWORD attribute, T value);

    static bool booleanWindowAttribute(HWND hwnd, DWORD attribute)
        { return QtDwmApiDll::windowAttribute<BOOL>(hwnd, attribute, FALSE) != FALSE; }

    static void setBooleanWindowAttribute(HWND hwnd, DWORD attribute, bool value)
        { setWindowAttribute<BOOL>(hwnd, attribute, BOOL(value ? TRUE : FALSE)); }

    DwmGetColorizationColor dwmGetColorizationColor;
    DwmSetWindowAttribute dwmSetWindowAttribute;
    DwmGetWindowAttribute dwmGetWindowAttribute;
    DwmExtendFrameIntoClientArea dwmExtendFrameIntoClientArea;
    DwmEnableBlurBehindWindow dwmEnableBlurBehindWindow;
    DwmIsCompositionEnabled dwmIsCompositionEnabled;
    DwmEnableComposition dwmEnableComposition;
    DwmSetIconicThumbnail dwmSetIconicThumbnail;
    DwmSetIconicLivePreviewBitmap dwmSetIconicLivePreviewBitmap;
    DwmInvalidateIconicBitmaps dwmInvalidateIconicBitmaps;
};

struct QtShell32Dll

{
    typedef HRESULT (STDAPICALLTYPE *SHCreateItemFromParsingName)(PCWSTR, IBindCtx *, REFIID, void **);
    typedef HRESULT (STDAPICALLTYPE *SetCurrentProcessExplicitAppUserModelID)(PCWSTR);

    QtShell32Dll() : sHCreateItemFromParsingName(0), setCurrentProcessExplicitAppUserModelID(0) {}

    void init()
    {
        if (!sHCreateItemFromParsingName && QSysInfo::windowsVersion() >= QSysInfo::WV_VISTA)
            resolve();
    }

    void resolve();

    SHCreateItemFromParsingName sHCreateItemFromParsingName; // Vista
    SetCurrentProcessExplicitAppUserModelID setCurrentProcessExplicitAppUserModelID; // Windows 7
};

extern QtDwmApiDll qtDwmApiDll;
extern QtShell32Dll qtShell32Dll;

inline void qt_qstringToNullTerminated(const QString &src, wchar_t *dst)
{
    dst[src.toWCharArray(dst)] = 0;
}

inline wchar_t *qt_qstringToNullTerminated(const QString &src)
{
    wchar_t *buffer = new wchar_t[src.length() + 1];
    qt_qstringToNullTerminated(src, buffer);
    return buffer;
}

template <class T>
T QtDwmApiDll::windowAttribute(HWND hwnd, DWORD attribute, T defaultValue)
{
    qtDwmApiDll.init();
    T value = defaultValue;
    if (qtDwmApiDll.dwmGetWindowAttribute)
        qtDwmApiDll.dwmGetWindowAttribute(hwnd, attribute, &value, sizeof(value));
    return value;
}

template <class T>
void QtDwmApiDll::setWindowAttribute(HWND hwnd, DWORD attribute, T value)
{
    qtDwmApiDll.init();
    if (qtDwmApiDll.dwmSetWindowAttribute)
        qtDwmApiDll.dwmSetWindowAttribute(hwnd, attribute, &value, sizeof(value));
}

QT_END_NAMESPACE

#endif // QWINFUNCTIONS_P_H
