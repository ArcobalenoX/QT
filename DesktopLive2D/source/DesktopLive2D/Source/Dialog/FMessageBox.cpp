#include "FMessageBox.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

FMessageBox::FMessageBox(const QString& title, const QString& text, int flag, QWidget *parent)
    : FlatWidget(parent)
    , return_value(true)
    , event_loop(nullptr)
{
    this->resize(250, 150);

    QVBoxLayout* vlayout = new QVBoxLayout();

    if ( flag == 0 )
    {
        label = new QLabel();
        vlayout->addWidget(label);
    }
    else if( flag == FMB_Yes )
    {
        label = new QLabel(center_widget);
        QPushButton* btn = new QPushButton(QString::fromLocal8Bit("确定"));

        connect(btn, SIGNAL(pressed()), this, SLOT(slotYesBtnClicked()));

        vlayout->addWidget(label);
        vlayout->addWidget(btn);

        this->SetLayout(vlayout);
    }
    else if( flag == FMB_YesCancel )
    {
        QHBoxLayout* hlayout = new QHBoxLayout();

        label = new QLabel(center_widget);
        QPushButton* btn_yes = new QPushButton(QString::fromLocal8Bit("确定"));
        QPushButton* btn_cancel = new QPushButton(QString::fromLocal8Bit("取消"));

        connect(btn_yes, SIGNAL(pressed()), this, SLOT(slotYesBtnClicked()));
        connect(btn_cancel, SIGNAL(pressed()), this, SLOT(slotCloseButtonClicked()));

        hlayout->addWidget(btn_yes);
        hlayout->addWidget(btn_cancel);

        vlayout->addWidget(label);
        vlayout->addLayout(hlayout);
    }

    label->setAlignment(Qt::AlignCenter);
    label->setWordWrap(true);

    this->SetLayout(vlayout);

    this->SetTitle(title);
    this->SetText(text);
}

void FMessageBox::SetTitle(const QString& title)
{
    this->title = title;
}

void FMessageBox::SetText(const QString& text)
{
    label->setText(text);
}

bool FMessageBox::Exec()
{
    /* 使用这个函数打开窗口为模态窗口 */
    this->setAttribute(Qt::WA_ShowModal, true);
    this->show();

    QEventLoop event_loop;
    this->event_loop = &event_loop;
    this->event_loop->exec();
    this->event_loop = nullptr;

    return return_value;
}

void FMessageBox::slotCloseButtonClicked()
{
    FlatWidget::slotCloseButtonClicked();

    return_value = false;
    if( event_loop ) event_loop->quit();
}

void FMessageBox::slotYesBtnClicked()
{
    FlatWidget::slotCloseButtonClicked();

    return_value = true;
    if( event_loop ) event_loop->quit();
}
