#ifndef NOTICEMANAGER_H
#define NOTICEMANAGER_H

#include <QObject>
#include <QMediaPlayer>

#include "PopupInfomationWidget.h"

class NoticeManager : public QObject
{
    Q_OBJECT

    struct SoundEffect
    {
        QString Path;
        QString Name;
    };

public:
    explicit NoticeManager(QObject *parent = 0);

    virtual ~NoticeManager();

    void SetPosition(const QPoint& position);

    void SetLinkWidget(QWidget* widget);

    void Move(const QPoint& position);

    void AppendMessage(const QString& msg, int exist_time_ms = 10000);

    void SetSoundEffectVolume(float ratio);

    void PlaySoundEffect();

public slots:
    void slotAppend(const QString& msg);

private:
    PopupInfomationWidget* info_widget;

    QMediaPlayer media_player;

    QPoint info_widget_position;
};

#endif // NOTICEMANAGER_H
