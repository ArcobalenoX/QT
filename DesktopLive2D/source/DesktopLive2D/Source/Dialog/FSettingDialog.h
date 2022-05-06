#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QWidget>

#include <Source/Common/FlatWidget.h>
#include <Source/Dialog/UISettingDialog.h>

class SettingDialog : public FlatWidget
{
    Q_OBJECT
public:
    explicit SettingDialog(QWidget *parent = 0);

    virtual ~SettingDialog();

    void SetCurrentModelPreview(QPixmap* preview);

signals:
    void sigFPSChanged(int level);

    void sigXFEnable(bool enable);

    void sigChangeModel();

public slots:
    void slotUpdatePreview();

private:
    Ui_SettingDialog* ui_setting_dialog;

    QPixmap* preview;
};

#endif // SETTINGDIALOG_H
