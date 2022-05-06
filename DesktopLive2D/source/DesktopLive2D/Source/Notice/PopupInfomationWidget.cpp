#include "PopupInfomationWidget.h"

#include <QPaintEvent>
#include <QPainterPath>


InfomationItem::InfomationItem(int dpi, int maxWidth, int exist_time_ms, QObject *parent) : QObject(parent)
{
    border_size = 8;

    /* 字体设置 */
    this->max_width = maxWidth - border_size * 2;
    font_weight = 10;
    font_width = dpi * font_weight / 72;
    font = QFont(QString::fromLocal8Bit("微软雅黑"), font_weight);

    is_exist_arrows = true;

    /* 创建定时器，使 item 在一定时间后消失 */
    timer = new QTimer(this);
    timer->setInterval(exist_time_ms);
    timer->setTimerType(Qt::VeryCoarseTimer);
    timer->start();

    connect(timer, SIGNAL(timeout()), this, SIGNAL(sigTimeOut()));
}

InfomationItem::~InfomationItem()
{
    //qDebug("delete");
}

void InfomationItem::SetArrows(bool exist)
{
    is_exist_arrows = exist;
}

QSize InfomationItem::GetSize()
{
    return size;
}

void InfomationItem::MoveTo(int x, int y)
{
    current_point.setX(x);
    current_point.setY(y);
}

void InfomationItem::ActionMoveTo(int x, int y)
{
    begin_point = current_point;
    end_point = QPoint(x, y);
}

void InfomationItem::SetText(const QString &text)
{
    this->text = text;

    int max_count = max_width / font_width;
    text_size.setWidth(qMin(max_count, text.size()) * font_width);
    text_size.setHeight((font_width * 3 / 2) * ::ceil(qreal(text.size()) / max_count));

    size.setWidth(text_size.width() + border_size * 2);
    size.setHeight(text_size.height() + border_size * 2 + 10);
}

void InfomationItem::Draw(QPainter &painter)
{
    this->DrawBorder(painter);
    this->DrawText(painter);
}

void InfomationItem::DrawBorder(QPainter &painter)
{
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(83, 190, 252));

    QPainterPath path;
    QRect rect(current_point.x(), current_point.y(), size.width(), size.height() - 10);
    path.addRoundedRect(rect, 10, 10);

    if(is_exist_arrows)
    {
        path.moveTo(rect.right() - 25, rect.bottom() + 1);
        path.lineTo(rect.right() -  0, rect.bottom() + 10);
        path.lineTo(rect.right() - 10, rect.bottom() + 1);
    }
    painter.drawPath(path);
    painter.restore();
}

void InfomationItem::DrawText(QPainter &painter)
{
    painter.save();
    painter.setPen(Qt::black);
    painter.setFont(font);
    painter.drawText(QRect(current_point.x() + border_size,
                           current_point.y() + border_size,
                           text_size.width(),
                           text_size.height()), this->text);
    painter.restore();
}

void InfomationItem::slotFrameChanged(int percent)
{
    qreal ratio = percent / 100.0;
    current_point = begin_point + (end_point - begin_point) * ratio;
}

//--------------------------------------------------------------
// PopupInfomationView
//--------------------------------------------------------------

PopupInfomationWidget::PopupInfomationWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);

    timeline.setDuration(300);
    timeline.setUpdateInterval(1000.0 / 60);
    timeline.setFrameRange(0, 100);
    timeline.setEasingCurve(QEasingCurve::OutSine);
    connect(&timeline, SIGNAL(frameChanged(int)), this, SLOT(slotStep(int)));

    this->resize(200, 400);
    dpi = QPaintDevice::logicalDpiX();
}

void PopupInfomationWidget::SetLinkWidget(QWidget* widget)
{
    widget->installEventFilter(this);
}

void PopupInfomationWidget::Clear()
{
    while(infomation_queue.isEmpty() == false)
    {
        infomation_queue.dequeue()->deleteLater();
    }
    this->update();
}

void PopupInfomationWidget::Clip(int count)
{
    /* 移除多余 */
    while(count > 0)
    {
        infomation_queue.dequeue()->deleteLater();
        count--;
    }
}

void PopupInfomationWidget::ShowAt(const QPoint& pos)
{
    this->ShowAt(pos.x(), pos.y());
}

void PopupInfomationWidget::ShowAt(int x, int y)
{
    this->move(x - this->width(), y - this->height());
    this->show();
}

void PopupInfomationWidget::AppendText(const QString &text, int exist_time_ms)
{
    InfomationItem *fi = new InfomationItem(dpi, this->width(), exist_time_ms, this);
    fi->SetText(text);

    connect(fi, SIGNAL(sigTimeOut()), this, SLOT(slotRemoveInfomationItem()));

    infomation_queue.enqueue(fi);

    int y = this->height() - fi->GetSize().height();
    int x = this->width()  - fi->GetSize().width();

    /* 创建第一个信息时，不执行动画 */
    if(infomation_queue.size() == 1)
    {
        fi->MoveTo(x, y);
        fi->SetArrows(true);
        this->update();
    }
    else
    {
        fi->MoveTo(x, this->height() + 5);
        int offsety = this->height();

        for(int i = infomation_queue.size() - 1; i >= 0; i--)
        {
            offsety = offsety - infomation_queue[i]->GetSize().height();
            x = this->width() - infomation_queue[i]->GetSize().width();
            infomation_queue[i]->ActionMoveTo(x, offsety);
            infomation_queue[i]->SetArrows(infomation_queue.size() - 1 == i);

            if(offsety < 0)
            {
                this->Clip(i + 1);
                break;
            }
        }
        if(timeline.state() & QTimeLine::Running) timeline.stop();
        timeline.start();
    }
}

void PopupInfomationWidget::slotStep(int num)
{
    for(int i = 0; i < infomation_queue.size(); i++)
    {
        infomation_queue[i]->slotFrameChanged(num);
    }
    this->update();
}

void PopupInfomationWidget::slotRemoveInfomationItem()
{
    infomation_queue.dequeue()->deleteLater();
    this->update();

    if( infomation_queue.isEmpty() )
    {
        this->close();
    }
}

void PopupInfomationWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 1));
    painter.drawRect(this->rect());

    for(int i = 0; i < infomation_queue.size(); i++)
    {
        infomation_queue[i]->Draw(painter);
    }
}

bool PopupInfomationWidget::eventFilter(QObject *, QEvent *event)
{
    if( this->isHidden() ) return false;

    /* 获取链接窗口的移动事件，更新弹出信息窗口位置 */
    if( event->type() == QEvent::Move )
    {
        QMoveEvent* move_event = static_cast<QMoveEvent*>(event);
        this->move(move_event->pos() - QPoint(this->width(), this->height()) + QPoint(50, 50));
    }
    return false;
}
