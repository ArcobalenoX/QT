#include "XFViewer.h"

#include <QDateTime>
#include <QDesktopWidget>
#include <QDesktopServices>

#include <Source/Common/DataManager.h>
#include <Source/Common/StyleSheetManager.h>


XFViewer::XFViewer(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);
    this->resize(250, 400);

    current_weel = -1;
    offset = 30;
    padding = 8;
    progress = 0;
    item_height = 40;

    /* 侧边隐藏 */
    hide_in_side = new HideInSide(this, this);
    hide_in_side->SetDiaplayRatio(0.2f);
    hide_in_side->SetEnable(Global::UserData.XFHideInSide);

    font.setFamily(QString::fromLocal8Bit("微软雅黑"));
    font.setPixelSize(12);

    QDesktopWidget desktop;
    this->move(desktop.screenGeometry().width() - this->width(), 0);

    /* 创建右键菜单 */
    QAction* action = nullptr;
    menu = new QMenu(this);
    menu->setStyleSheet(StyleSheetManager::GetMenuStyle());

    action = menu->addAction(QString::fromLocal8Bit("移动到右上角"));
    connect(action, &QAction::triggered, this, [=](bool){
        QDesktopWidget desktop;
        this->move(desktop.screenGeometry().width() - this->width(), 0);

        Global::UserData.XFViewPoint = this->pos();
        DataManager::GetInstance()->Update();
    });

    action = menu->addAction(QString::fromLocal8Bit("可移动窗口"));
    action->setCheckable(true);
    action->setChecked(Global::UserData.XFMovable);
    connect(action, &QAction::toggled, this, [=](bool on){
        Global::UserData.XFMovable = on;
        DataManager::GetInstance()->Update();
    });

    action = menu->addAction(QString::fromLocal8Bit("屏幕边缘隐藏"));
    action->setCheckable(true);
    action->setChecked(Global::UserData.XFHideInSide);
    connect(action, &QAction::toggled, this, [=](bool on){
        Global::UserData.XFHideInSide = on;
        DataManager::GetInstance()->Update();
        hide_in_side->Enter();
        hide_in_side->SetEnable(Global::UserData.XFHideInSide);
    });

    menu->addAction(QString::fromLocal8Bit("选择新番"), this, SIGNAL(sigSelectedXF()));
    menu->addSeparator();
    menu->addAction(QString::fromLocal8Bit("退出"), this, SLOT(close()));

#ifdef XFVIEWER_DEBUG
    timer.setInterval(1000 / 6);
    timer.start();

    connect(&timer, SIGNAL(timeout()), this, SLOT(slotTest()));

    cur_date_time.setDate(QDate(2017, 10, 15));
    cur_date_time.setTime(QTime(0, 0, 0));
#endif
}

#ifdef XFVIEWER_DEBUG
void XFViewer::slotTest()
{
    cur_date_time = cur_date_time.addSecs(60 * 10);
    this->UpdateViewer();
}
#endif

int XFViewer::GetWeek(const QDateTime& dt)
{
    static const int week_count = 7;
    static QString convert[week_count] = {
        QString::fromLocal8Bit("星期一"),
        QString::fromLocal8Bit("星期二"),
        QString::fromLocal8Bit("星期三"),
        QString::fromLocal8Bit("星期四"),
        QString::fromLocal8Bit("星期五"),
        QString::fromLocal8Bit("星期六"),
        QString::fromLocal8Bit("星期日")
    };

    int week = -1;
    QString week_str = dt.toString("dddd");

    for(int i = 0; i < week_count; i++)
    {
        if( convert[i].compare(week_str) == 0 )
        {
            week = i + 1; break;
        }
    }
    return week;
}

void XFViewer::SetXFDatas(const XFDataVector& datas)
{
    xf_datas = datas;

#ifdef XFVIEWER_DEBUG
    this->CreateItems(this->GetWeek(cur_date_time));
#else
    this->CreateItems(this->GetWeek(QDateTime::currentDateTime()));
#endif
}

void XFViewer::CreateItems(int week)
{
    item_vector.clear();

    next_index = -1;
    current_weel = week;

    for(int i = 0; i < xf_datas.size(); i++)
    {
        if( xf_datas[i].Index >= week * 10000 && xf_datas[i].Index < (week + 1) * 10000 )
        {
            XFViewerItem item;
            item.Title   = xf_datas[i].Title;
            item.Time    = xf_datas[i].DateTime;
            item.TimeMin = xf_datas[i].Index % 10000;
            item.Index   = i;
            item.Update  = false;
            item_vector << item;
        }
    }
    is_no_xf = item_vector.isEmpty();

    this->resize(this->width(), qMax(1, item_vector.size()) * item_height + offset + padding);
    this->UpdateViewer();
}

void XFViewer::UpdateViewer()
{
#ifdef XFVIEWER_DEBUG
    QDateTime date_time = cur_date_time;
    time = date_time.toString("yyyy-MM-dd  hh:mm  dddd");
#else
    QDateTime date_time = QDateTime::currentDateTime();
    time = date_time.toString("yyyy-MM-dd  hh:mm  dddd");
#endif

    /* 到了第二天 */
    int week = this->GetWeek(date_time);
    if( current_weel != week )
    {
        this->CreateItems(week);
    }

    if( is_no_xf )
    {
        this->update();  return;
    }

    int index = -1;
    int cur_time = date_time.toString("hh").toInt() * 60 + date_time.toString("mm").toInt();

    for(int i = 0; i < item_vector.size(); i++)
    {
        if( item_vector[i].TimeMin > cur_time )
        {
            index = i; break;
        }
        item_vector[i].Update = true;
    }

    /* 发出新番更新信号 */
    if( next_index != index )
    {
        next_index = index;
        if( index != 0 )
        {
            int idx = (index == -1) ? item_vector.size() : index;
            emit sigUpdateXF(item_vector[idx - 1].Title);
        }
    }

    /* index 为 -1 时表示当天所有新番更新完毕 */
    int prev_time, next_time;
    if( index == -1 )
    {
        prev_time = item_vector[item_vector.size() - 1].TimeMin;
        next_time = 24 * 60;
    }
    else
    {
        prev_time = (index == 0) ? 0 : item_vector[index - 1].TimeMin;
        next_time = item_vector[index].TimeMin;
    }
    float ratio = (float)(cur_time - prev_time) / (next_time - prev_time);

    /* 已经更新的新番的数量 */
    int update_xf_count = (index == -1) ? item_vector.size() : index;

    /* 计算时间刻度的进度 */
    if( update_xf_count == 0 )
    {
        progress = qMin(this->height() - padding, int(offset + item_height * 0.5f * ratio));
    }
    else
    {
        int base_height = item_height * update_xf_count - item_height / 2;
        progress = qMin(this->height() - padding, int(offset + base_height + item_height * ratio));
    }

    this->update();
}

int XFViewer::GetHoverItemIndex(const QPoint& point)
{
    int index = (point.y() - offset) / item_height;
    if( point.y() < offset ) index = -1;

    return index;
}

void XFViewer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 200));
    painter.fillRect(QRect(0, 0, this->width(), progress), QColor(0, 0, 0, 100));

    /* 绘制时间 */
    font.setPixelSize(13);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(QRect(0, 0, this->width(), offset).adjusted(5, 3, -5, -3), Qt::AlignVCenter | Qt::AlignLeft, time);

    if( is_no_xf )
    {
        font.setPixelSize(15);
        painter.setFont(font);
        painter.drawText(this->rect().adjusted(0, offset, 0, 0), Qt::AlignCenter, QString::fromLocal8Bit("今天没有新番更新"));
        return;
    }

    int linex = 50;

    painter.setPen(QColor(52, 152, 219));
    painter.drawLine(linex, offset, linex, progress);

    painter.setPen(Qt::white);
    painter.drawLine(linex, progress, linex, this->height() - padding);

    painter.setPen(QColor(52, 152, 219, 120));
    painter.drawLine(padding, progress, this->width() - padding, progress);

    painter.setRenderHint(QPainter::Antialiasing);

    for(int i = 0; i < item_vector.size(); i++)
    {
        QRect rect(0, offset + item_height * i, this->width(), item_height);

        QColor color = item_vector[i].Update ? QColor(52, 152, 219) : Qt::white;
        color = (i == current_index ) ? QColor(46, 204, 113) : color;

        font.setPixelSize(12);
        painter.setFont(font);
        painter.setPen(color);
        painter.drawText(rect.adjusted(padding, 0, 0, 0), Qt::AlignVCenter | Qt::AlignLeft, item_vector[i].Time);

        int circle_radius = 3;
        int circle_size = circle_radius * 2 + 1;

        QPainterPath circle;
        circle.addEllipse(linex - circle_radius, (rect.top() + rect.bottom()) * 0.5f - circle_radius, circle_size, circle_size);
        painter.fillPath(circle, color);

        font.setPixelSize(14);
        painter.setFont(font);
        painter.drawText(rect.adjusted(linex + 10, 0, -padding, 0), Qt::AlignVCenter | Qt::AlignLeft, item_vector[i].Title);
    }
}

void XFViewer::mousePressEvent(QMouseEvent *event)
{
    is_move_window = false;

    mouse_origin_point = event->globalPos();
    window_origin_point = this->pos();

    /* 弹出菜单 */
    if( event->button() & Qt::RightButton )
    {
        menu->exec(QCursor::pos());
    }
}

void XFViewer::mouseDoubleClickEvent(QMouseEvent *event)
{
    if( event->button() != Qt::LeftButton ||  is_no_xf ) return;

    /* 使用浏览器打开新番页面 */
    int index = this->GetHoverItemIndex(event->pos());
    if( index != -1 )
    {
        QString url = xf_datas[item_vector[index].Index].CoverUrl;
        QDesktopServices::openUrl(QUrl(url));
    }
}

void XFViewer::mouseMoveEvent(QMouseEvent *event)
{
    is_move_window = true;

    if( Global::UserData.XFMovable && event->buttons() & Qt::LeftButton )
    {
        QPoint offset = event->globalPos() - mouse_origin_point;
        this->move(window_origin_point + offset);
    }

    if( is_no_xf ) return;

    current_index = this->GetHoverItemIndex(event->pos());
    this->update();
}

void XFViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if( is_move_window && event->button() & Qt::LeftButton )
    {
        Global::UserData.XFViewPoint = this->pos();
        DataManager::GetInstance()->Update();
    }
}

void XFViewer::leaveEvent(QEvent *)
{
    current_index = -1;
    this->update();
}
