#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QSystemTrayIcon>

#include "DesktopLive2D.h"

#include "Common/DropdownMenu.h"
#include "Notice/NoticeManager.h"

#include "Dialog/FSettingDialog.h"
#include "Capture/CaptureTool.h"
#include "Bilibili/XFManager.h"

class Application : public QObject
{
    Q_OBJECT
public:
    explicit Application(QObject *parent = 0);

    virtual ~Application();

    bool Init();

private:
    void LoadConfigData();

    void CreateSystemTrayIcon();

    void CreateXFManager();

    void RemoveXFManager();

public slots:
    void slotSaveConfigData();

    void slotDropdownMenuItemClicked(int index);

    void slotRunAfterStartUp(bool on);

private:
    XFManager* xf_manager;
    DesktopLive2D* desktop_live2d;

    NoticeManager* notice_manager;

    /* 截图工具 */
    CaptureTool* capture_tool;

    /* 设置窗口 */
    SettingDialog* setting_dialog;

    /* 下拉菜单 */
    DropdownMenu* dropdown_menu;

    /* 托盘图标 */
    QSystemTrayIcon* tray_icon;

    QAction* act_xf_enable;
};

#endif // APPLICATION_H
