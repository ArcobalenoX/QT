#include "Application.h"

#include "Common/PathHelper.h"
#include "Common/DataManager.h"
#include "Common/StyleSheetManager.h"

#include <QSettings>
#include <QApplication>

Application::Application(QObject *parent)
    : QObject(parent)
    , xf_manager(nullptr)
    , capture_tool(nullptr)
    , setting_dialog(nullptr)
    , notice_manager(nullptr)
{
    this->LoadConfigData();
}

Application::~Application()
{
    this->slotSaveConfigData();
}

bool Application::Init()
{
    desktop_live2d = new DesktopLive2D(this);
    if( desktop_live2d->Init() == false ) return false;

    /* 创建托盘图标 */
    this->CreateSystemTrayIcon();

    notice_manager = new NoticeManager(this);
    notice_manager->SetLinkWidget(desktop_live2d->GetRenderWidget());

    /* 创建下拉菜单 */
    dropdown_menu  = desktop_live2d->GetDropdownMenu();
    connect(dropdown_menu, SIGNAL(sigClicked(int)), this, SLOT(slotDropdownMenuItemClicked(int)));

    if( Global::UserData.XFEnable )
    {
        this->CreateXFManager();
    }
    return true;
}

void Application::CreateSystemTrayIcon()
{
    tray_icon = new QSystemTrayIcon(QIcon(PathHelper::GetIconDirPath() + "icon_desktop_32.png"), this);
    tray_icon->setToolTip("DesktopLive2D");
    tray_icon->show();

    /* 创建托盘菜单 */
    QMenu* menu = new QMenu;
    menu->setStyleSheet(StyleSheetManager::GetMenuStyle());

    act_xf_enable = menu->addAction(QString::fromLocal8Bit("开启追番工具"));
    act_xf_enable->setCheckable(true);
    act_xf_enable->setChecked(Global::UserData.XFEnable);
    connect(act_xf_enable, &QAction::toggled, this, [=](bool on){
        Global::UserData.XFEnable = on;
        DataManager::GetInstance()->Update();

        if( on )
        {
            this->CreateXFManager();
        }
        else
        {
            this->RemoveXFManager();
        }
    });

    menu->addAction(QString::fromLocal8Bit("退出"), qApp, SLOT(quit()));

    tray_icon->setContextMenu(menu);
}

void Application::CreateXFManager()
{
    if( xf_manager == nullptr )
    {
        xf_manager = new XFManager(this);

        connect(xf_manager, &XFManager::sigUpdateXF, this, [=](const QString& str){
            QString info = QString::fromLocal8Bit("< %1 > - 已经更新").arg(str);
            notice_manager->AppendMessage(info);
        });
    }
}

void Application::RemoveXFManager()
{
    if( xf_manager )
    {
        delete xf_manager;
        xf_manager = nullptr;
    }
}

void Application::LoadConfigData()
{
    connect(DataManager::GetInstance(), SIGNAL(sigUpdate()), this, SLOT(slotSaveConfigData()));

    DataSet* data_set = DataManager::GetInstance()->GetDataSet("Config");

    if( data_set->Size() == 0 ){
        /* 设置默认数据 */
        Global::UserData.BackgroundColor   = QColor(0, 0, 0, 25);
        Global::UserData.DisplayBackground = true;
        Global::UserData.HoverBackground   = true;
        Global::UserData.FPS               = 1;
        Global::UserData.OpenSound         = true;
        Global::UserData.ResizeModelWindow = false;
        Global::UserData.ModelWindowSize   = QSize(MODEL_WINDOW_SIZE, MODEL_WINDOW_SIZE);
        Global::UserData.ModelWindowPos    = QPoint(0, 0);
        Global::UserData.CurrentModelName  = "shizuku";
        Global::UserData.WindowStaysOnTop  = false;

        Global::UserData.XFEnable          = true;
        Global::UserData.XFMovable         = true;
        Global::UserData.XFViewPoint       = QPoint(0, 0);
        Global::UserData.XFHideInSide      = false;

        this->slotSaveConfigData();
    }
    else{
        Global::UserData.RunAfterStartUp = false;

        Global::UserData.BackgroundColor   = data_set->GetValue("BackgroundColor").value<QColor>();
        Global::UserData.HoverBackground   = data_set->GetValue("HoverBackground").toBool();
        Global::UserData.DisplayBackground = data_set->GetValue("DisplayBackground").toBool();
        Global::UserData.FPS               = data_set->GetValue("FPS").toInt();
        Global::UserData.OpenSound         = data_set->GetValue("OpenSound").toBool();
        Global::UserData.ResizeModelWindow = data_set->GetValue("ResizeModelWindow").toBool();
        Global::UserData.ModelWindowSize   = data_set->GetValue("ModelWindowSize").toSize();
        Global::UserData.ModelWindowPos    = data_set->GetValue("ModelWindowPos").toPoint();
        Global::UserData.CurrentModelName  = data_set->GetValue("CurrentModelName").toString();
        Global::UserData.WindowStaysOnTop  = data_set->GetValue("WindowStaysOnTop").toBool();

        Global::UserData.XFEnable          = data_set->GetValue("XFEnable").toBool();
        Global::UserData.XFMovable         = data_set->GetValue("XFMovable").toBool();
        Global::UserData.XFViewPoint       = data_set->GetValue("XFViewPoint").toPoint();
        Global::UserData.XFHideInSide      = data_set->GetValue("XFHideInSide").toBool();
    }
}

void Application::slotSaveConfigData()
{
    DataSet* data_set = DataManager::GetInstance()->GetDataSet("Config");

    data_set->AddValue("BackgroundColor", Global::UserData.BackgroundColor);
    data_set->AddValue("HoverBackground", Global::UserData.HoverBackground);
    data_set->AddValue("DisplayBackground", Global::UserData.DisplayBackground);
    data_set->AddValue("FPS", Global::UserData.FPS);
    data_set->AddValue("OpenSound", Global::UserData.OpenSound);
    data_set->AddValue("ResizeModelWindow", Global::UserData.ResizeModelWindow);
    data_set->AddValue("ModelWindowSize", Global::UserData.ModelWindowSize);
    data_set->AddValue("ModelWindowPos", Global::UserData.ModelWindowPos);
    data_set->AddValue("CurrentModelName", Global::UserData.CurrentModelName);
    data_set->AddValue("WindowStaysOnTop", Global::UserData.WindowStaysOnTop);

    data_set->AddValue("XFEnable", Global::UserData.XFEnable);
    data_set->AddValue("XFMovable", Global::UserData.XFMovable);
    data_set->AddValue("XFViewPoint", Global::UserData.XFViewPoint);
    data_set->AddValue("XFHideInSide", Global::UserData.XFHideInSide);

    DataManager::GetInstance()->SaveDataSet();
}

void Application::slotDropdownMenuItemClicked(int index)
{
    if( index == 0 && capture_tool == nullptr ){
        /* 截图工具 */
        capture_tool = new CaptureTool;
        capture_tool->show();

        connect(capture_tool, &CaptureTool::destroyed, this, [=](QObject*){
            capture_tool = nullptr;
        });
    }
    else if( index == 1 ){
        /* 音乐播放器 */
    }
    else if( index == 2 && setting_dialog == nullptr ){
        /* 设置窗口 */
        setting_dialog = new SettingDialog();
        setting_dialog->SetCurrentModelPreview(desktop_live2d->GetCurrentModelPreview());
        setting_dialog->SetDeleteAfterClose(true);
        setting_dialog->show();

        connect(setting_dialog, &SettingDialog::sigXFEnable, this, [=](bool enable){
            if( enable )
            {
                this->CreateXFManager();
            }
            else
            {
                this->RemoveXFManager();
            }
            act_xf_enable->setChecked(enable);
        });

        connect(setting_dialog, SIGNAL(sigFPSChanged(int)), desktop_live2d, SLOT(slotSetModelFPS(int)));
        connect(setting_dialog, SIGNAL(sigChangeModel()), desktop_live2d, SLOT(slotOpenModelSelectWidget()));

        /* 更换模型时，通知设置窗口刷新模型预览图 */
        connect(desktop_live2d, SIGNAL(sigChangedModel()), setting_dialog, SLOT(slotUpdatePreview()));

        connect(setting_dialog, &SettingDialog::sigCloseButtonClicked, this, [=]{
            setting_dialog = nullptr;
        });
    }
    else if( index == 3 ){
        qApp->quit();   /* 退出 */
    }
}

void Application::slotRunAfterStartUp(bool on)
{
    Global::UserData.RunAfterStartUp = on;
    DataManager::GetInstance()->Update();

    /* 开机启动程序 */
    QString reg_run  = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    QString app_name = QApplication::applicationName();

    /* 修改注册表实现开机自启 */
    QSettings* setting = new QSettings(reg_run, QSettings::NativeFormat);
    if( on ){
        QString app_path = QApplication::applicationFilePath();
        setting->setValue(app_name, app_path.replace("/", "\\"));
    }
    else{
        setting->remove(app_name);
    }
    delete setting;
}
