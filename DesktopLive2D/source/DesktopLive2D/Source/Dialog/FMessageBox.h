#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QLabel>
#include <QWidget>
#include <QEventLoop>

#include <Source/Common/FlatWidget.h>

enum FMB_Button
{
    FMB_Null,
    FMB_Yes,
    FMB_YesCancel
};

class FMessageBox : public FlatWidget
{
    Q_OBJECT
public:
    explicit FMessageBox(const QString& title = "", const QString& text = "", int flag = 0, QWidget *parent = 0);

    void SetTitle(const QString& title);

    void SetText(const QString& text);

    bool Exec();

public slots:
    void slotCloseButtonClicked();

    void slotYesBtnClicked();

private:
    QLabel* label;
    QEventLoop* event_loop;

    bool is_modal;
    bool return_value;
};

#endif // MESSAGEBOX_H
