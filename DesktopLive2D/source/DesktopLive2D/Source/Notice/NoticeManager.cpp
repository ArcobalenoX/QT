#include "NoticeManager.h"

#include <Source/Common.h>
#include <Source/Common/PathHelper.h>

NoticeManager::NoticeManager(QObject *parent) : QObject(parent)
{
    info_widget = new PopupInfomationWidget();

    media_player.setMedia(QUrl(PathHelper::GetSoundDirPath() + "poi.wav"));
}

NoticeManager::~NoticeManager()
{
    delete info_widget;
}

void NoticeManager::SetPosition(const QPoint& position)
{
    info_widget_position = position;
}

void NoticeManager::SetLinkWidget(QWidget* widget)
{
    info_widget->SetLinkWidget(widget);
}

void NoticeManager::Move(const QPoint& position)
{
    if( info_widget->isHidden() == false )
    {
        info_widget_position = position;

        int x = info_widget_position.x() - info_widget->width();
        int y = info_widget_position.y() - info_widget->height();

        info_widget->move(x, y);
    }
}

void NoticeManager::AppendMessage(const QString& msg, int exist_time_ms)
{
    this->SetPosition(Global::UserData.ModelWindowPos + QPoint(50, 50));
    if( info_widget->isHidden() )
    {
        int x = info_widget_position.x() - info_widget->width();
        int y = info_widget_position.y() - info_widget->height();

        info_widget->move(x, y);
        info_widget->show();
    }
    info_widget->AppendText(msg, exist_time_ms);

    this->PlaySoundEffect();
}

void NoticeManager::slotAppend(const QString& msg)
{
    this->AppendMessage(msg);
}

void NoticeManager::SetSoundEffectVolume(float ratio)
{
    media_player.setVolume(ratio * 100);
}

void NoticeManager::PlaySoundEffect()
{
    media_player.stop();
    media_player.play();
}
