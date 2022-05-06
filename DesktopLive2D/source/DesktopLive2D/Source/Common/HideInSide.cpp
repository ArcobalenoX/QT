#include "HideInSide.h"

#include <QDesktopWidget>

HideInSide::HideInSide(QWidget *obj, QObject *parent)
    : QObject(parent)
    , widget(obj)
    , is_show(true)
    , is_finished(true)
{
    /* 安装事件过滤器，截取 enterEvent 和 leaveEvent 事件 */
    widget->installEventFilter(this);

    timeLine.setDuration(250);
    timeLine.setUpdateInterval(1000 / 60.0);    /* 30 帧每秒 */
    timeLine.setFrameRange(0, 100);

    connect(&timeLine, SIGNAL(frameChanged(int)), this, SLOT(slotStep(int)));
    timeLine.stop();

    QDesktopWidget w;
    screen_size = w.screenGeometry().size();

    begin_point = end_point = current_point = QPoint(0, 0);
    this->SetDiaplayRatio(0.5f);
}

HideInSide::~HideInSide()
{

}

void HideInSide::Enter()
{
    if( is_enable == false ) return;

    if( is_finished == false )
    {
        timeLine.stop();
    }
    this->Show();
}

void HideInSide::Leave()
{
    if( is_enable == false ) return;

    if( is_finished == false )
    {
        timeLine.stop();
    }
    this->Hide();
}

void HideInSide::Show()
{
    if( is_show || GetCurrentDirection() == Direction::None ) return;
    is_show = true;

    this->CalcShowValue();
    this->StartAnimation();
}

void HideInSide::Hide()
{
    if( is_show == false || GetCurrentDirection() == Direction::None ) return;
    is_show = false;

    this->CalcHideValue();
    this->StartAnimation();
}

HideInSide::Direction HideInSide::GetCurrentDirection()
{
    QPoint pos = widget->pos();
    direction = Direction::None;

    if(pos.x() + widget->width() >= screen_size.width())
    {
        direction = Direction::Right;
    }
    else if(pos.x() <= 0)
    {
        direction = Direction::Left;
    }
    else if(pos.y() <= 0)
    {
        direction = Direction::Top;
    }
    else if(pos.y() + widget->height() >= screen_size.height())
    {
        direction = Direction::Bottom;
    }
    return direction;
}

void HideInSide::CalcHideValue()
{
    end_point = begin_point = widget->pos();

    if(direction == Direction::Top)
    {
        end_point.setY(-widget->height() * (1 - display_ratio));
    }
    else if(direction == Direction::Bottom)
    {
        end_point.setY(screen_size.height() - widget->height() * display_ratio);
    }
    else if(direction == Direction::Left)
    {
        end_point.setX(-widget->width() * (1 - display_ratio));
    }
    else if(direction == Direction::Right)
    {
        end_point.setX(screen_size.width() - widget->width() * display_ratio);
    }
}

void HideInSide::CalcShowValue()
{
    end_point = begin_point = widget->pos();

    if(direction == Direction::Top)
    {
        end_point.setY(0);
    }
    else if(direction == Direction::Bottom)
    {
        end_point.setY(screen_size.height() - widget->height());
    }
    else if(direction == Direction::Left)
    {
        end_point.setX(0);
    }
    else if(direction == Direction::Right)
    {
        end_point.setX(screen_size.width() - widget->width());
    }
}

void HideInSide::StartAnimation()
{
    is_finished = false;
    timeLine.start();
}

void HideInSide::slotStep(int value)
{
    qreal percent = value / 100.0;
    current_point.setX(begin_point.x() + (end_point.x() - begin_point.x()) * percent);
    current_point.setY(begin_point.y() + (end_point.y() - begin_point.y()) * percent);

    widget->move(current_point);

    if( percent == 1 )
    {
        is_finished = true;
    }
}

bool HideInSide::eventFilter(QObject *, QEvent *event)
{
    if( event->type() == QEvent::Enter )
    {
        this->Enter();
    }
    else if( event->type() == QEvent::Leave )
    {
        this->Leave();
    }
    return false;
}

