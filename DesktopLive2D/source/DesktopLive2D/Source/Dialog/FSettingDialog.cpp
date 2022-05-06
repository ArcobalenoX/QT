#include "FSettingDialog.h"

#include <Source/Common.h>
#include <Source/Common/DataManager.h>

#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>

SettingDialog::SettingDialog(QWidget *parent) : FlatWidget(parent)
{
    this->SetWindowTitle(QString::fromLocal8Bit("ÉèÖÃ"));

    ui_setting_dialog = new Ui_SettingDialog;
    ui_setting_dialog->setupUi(this->GetCenterWidget());

    this->resize(this->GetCenterWidget()->size() +
                 QSize(this->GetPadding() * 2, this->GetHeadHeight() + this->GetPadding() * 2));

    ui_setting_dialog->btn_bg_color->setStyleSheet(
                QString("QToolButton{ background-color: %1; }").arg("black"));

    /* FPS */
    switch (Global::UserData.FPS) {
    case 0: ui_setting_dialog->radio_btn_30->setChecked(true); break;
    case 1: ui_setting_dialog->radio_btn_48->setChecked(true); break;
    case 2: ui_setting_dialog->radio_btn_60->setChecked(true); break;
    }

    connect(ui_setting_dialog->radio_btn_30, &QRadioButton::pressed, this, [=](){
        Global::UserData.FPS = 0;
        DataManager::GetInstance()->Update();
        emit sigFPSChanged(Global::UserData.FPS);
    });
    connect(ui_setting_dialog->radio_btn_48, &QRadioButton::pressed, this, [=](){
        Global::UserData.FPS = 1;
        DataManager::GetInstance()->Update();
        emit sigFPSChanged(Global::UserData.FPS);
    });
    connect(ui_setting_dialog->radio_btn_60, &QRadioButton::pressed, this, [=](){
        Global::UserData.FPS = 2;
        DataManager::GetInstance()->Update();
        emit sigFPSChanged(Global::UserData.FPS);
    });


    /* ÇÐ»»Ä£ÐÍ */
    connect(ui_setting_dialog->btn_changed_model, SIGNAL(pressed()), this, SIGNAL(sigChangeModel()));

    /* model widget */
    ui_setting_dialog->cb_open_sound->setChecked(Global::UserData.OpenSound);
    connect(ui_setting_dialog->cb_open_sound, &QCheckBox::toggled, this, [=](bool check){
        Global::UserData.OpenSound = check;
        DataManager::GetInstance()->Update();
    });

    ui_setting_dialog->cb_show_background->setChecked(Global::UserData.DisplayBackground);
    connect(ui_setting_dialog->cb_show_background, &QCheckBox::toggled, this, [=](bool check){
        Global::UserData.DisplayBackground = check;
        DataManager::GetInstance()->Update();
    });

    ui_setting_dialog->cb_hover_background->setChecked(Global::UserData.HoverBackground);
    connect(ui_setting_dialog->cb_hover_background, &QCheckBox::toggled, this, [=](bool check){
        Global::UserData.HoverBackground = check;
        DataManager::GetInstance()->Update();
    });

    ui_setting_dialog->cb_resize_window->setChecked(Global::UserData.ResizeModelWindow);
    connect(ui_setting_dialog->cb_resize_window, &QCheckBox::toggled, this, [=](bool check){
        Global::UserData.ResizeModelWindow = check;
        DataManager::GetInstance()->Update();
    });

    /* other */
    ui_setting_dialog->cb_startup->setChecked(Global::UserData.RunAfterStartUp);
    connect(ui_setting_dialog->cb_startup, &QCheckBox::toggled, this, [=](bool check){
        Global::UserData.RunAfterStartUp = check;
        DataManager::GetInstance()->Update();
    });

    ui_setting_dialog->cb_enable_xf->setChecked(Global::UserData.XFEnable);
    connect(ui_setting_dialog->cb_enable_xf, &QCheckBox::toggled, this, [=](bool check){
        Global::UserData.XFEnable = check;
        DataManager::GetInstance()->Update();

        emit sigXFEnable(check);
    });
}

SettingDialog::~SettingDialog()
{
    delete ui_setting_dialog;
}

void SettingDialog::SetCurrentModelPreview(QPixmap* preview)
{
    this->preview = preview;
    ui_setting_dialog->preview->setScaledContents(true);
    ui_setting_dialog->preview->setPixmap(*preview);
}

void SettingDialog::slotUpdatePreview()
{
    ui_setting_dialog->preview->setPixmap(*preview);
    ui_setting_dialog->preview->update();
}
