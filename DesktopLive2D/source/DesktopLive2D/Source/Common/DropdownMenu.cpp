#include "DropdownMenu.h"

static const int DefaultItemSize = 32;
static const int DefaultItemSpace = 10;

static const QColor ClickedColor   = QColor(39, 174, 96, 200);
static const QColor HighLightColor = QColor(46, 204, 113, 200);
static const QColor DefaultBGColor = QColor(0, 0, 0, 200);

//----------------------------------------------------------------------
// DropdownMenuItem
//----------------------------------------------------------------------
DropdownMenuItem::DropdownMenuItem(const QString &filename)
{
    opacity = 1.0f;
    icon.load(filename);
}

void DropdownMenuItem::Paint(QPainter& painter, bool active, bool clicked)
{
    painter.setOpacity(opacity);

    painter.setPen(Qt::NoPen);
    painter.setBrush(active ? (clicked ? ClickedColor : HighLightColor) : DefaultBGColor);

    QRect rect(position.x(), position.y(), item_size, item_size);

    QPainterPath circle;
    circle.addEllipse(rect);
    painter.drawPath(circle);

    int padding = 4;
    painter.drawImage(rect.adjusted(padding, padding, -padding, -padding), icon);
}

//----------------------------------------------------------------------
// Tiwidget
//----------------------------------------------------------------------
Tiwidget::Tiwidget(QWidget* parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->resize(0, DefaultItemSize);

    int sec_time = 250;
    anim_move = new QPropertyAnimation(this, "pos", this);
    anim_move->setEasingCurve(QEasingCurve::OutSine);
    anim_move->setDuration(sec_time);

    int font_weight = 12;
    font = QFont(QString::fromLocal8Bit("微软雅黑"), font_weight);

    /* dpi * weight / 72 */
    font_width = QPaintDevice::logicalDpiX() * font_weight / 72;
}

void Tiwidget::SetText(const QString& text)
{
    this->text = text;
    this->update();
}

int Tiwidget::CalcWidgetWidth(const QString& text)
{
    return (text.size() + 2) * font_width;
}

void Tiwidget::UpdateWidth(const QString& text)
{
    this->resize(qMax(this->width(), this->CalcWidgetWidth(text)), this->height());
}

bool Tiwidget::ShowInLeft(const QPoint& pos)
{
    return pos.x() - this->width() - 10 > 0;
}

void Tiwidget::ShowAt(QPoint& pos)
{
    if( anim_move->state() == QPropertyAnimation::Running ) anim_move->stop();

    if( this->ShowInLeft(pos) )
    {
        pos.setX(pos.x() - this->width() - 10);
    }
    else
    {
        pos.setX(pos.x() + this->height() + 10);
    }
    this->move(pos.x(), this->pos().y());

    anim_move->setStartValue(this->pos());
    anim_move->setEndValue(pos);
    anim_move->start();

    if( this->isHidden() ) this->show();
}

void Tiwidget::paintEvent(QPaintEvent *)
{
    int h = this->height();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(DefaultBGColor);
    painter.drawRoundedRect(this->rect(), h * 0.5f, h);

    painter.setPen(Qt::white);
    painter.setFont(font);
    painter.drawText(this->rect().adjusted(h * 0.5f, 4, -h * 0.5f, -4), Qt::AlignHCenter, text);
}

//----------------------------------------------------------------------
// DropdownMenu
//----------------------------------------------------------------------
DropdownMenu::DropdownMenu(QWidget* parent)
    : QWidget(parent)
    , is_first_tick(true)
    , is_show_tip_widget(true)
    , is_enter(false)
    , is_clicked(false)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

    /* 由于要执行动画，需要定时刷新窗口 */
    timer = new QTimer(this);
    timer->setInterval(1000.0 / 48);
    timer->start();
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    /* 通过属性动画来实现菜单的显示、隐藏动画 */
    anim_show_menu = new QParallelAnimationGroup(this);
    anim_hide_menu = new QPropertyAnimation(this, "windowOpacity", this);
    anim_hide_menu->setDuration(160);
    anim_hide_menu->setStartValue(1.0f);
    anim_hide_menu->setEndValue(0.0f);
    connect(anim_hide_menu, SIGNAL(finished()), this, SLOT(close()));

    tip_widget = new Tiwidget;

    item_size = DefaultItemSize;
    item_space = DefaultItemSpace;
    current_item = -1;

    this->resize(item_size, item_size);
}

DropdownMenu::~DropdownMenu()
{
    delete tip_widget;

    for(int i = 0; i < item_vector.size(); i++)
    {
        delete item_vector[i];
    }
    item_vector.clear();
}

void DropdownMenu::AddItem(const QString& filename, const QString &text)
{
    DropdownMenuItem* item = new DropdownMenuItem(filename);
    item->SetItemSize(item_size);
    item->SetText(text);
    this->AddItem(item);

    tip_widget->UpdateWidth(text);
}

void DropdownMenu::AddItem(DropdownMenuItem* item)
{
    item_vector.push_back(item);
}

void DropdownMenu::ShowMenu(const QPoint& pos)
{
    if( this->isHidden() == false ) return;

    if( is_first_tick )
    {
        is_first_tick = false;
        this->CreateAnimation();
    }

    anim_show_menu->stop();
    this->UpdateItemsPosition();
    anim_show_menu->start();

    this->setWindowOpacity(1.0f);
    this->move(pos.x() - this->width(), pos.y());
    this->show();
    /* 设置焦点，待失去焦点时 focusOutEvent() 窗口隐藏 */
    this->setFocus();

    tip_widget->move(0, this->pos().y());
}

void DropdownMenu::HideMenu()
{
    if( this->isHidden() ) return;

    anim_hide_menu->stop();
    anim_hide_menu->start();

    tip_widget->hide();
}

void DropdownMenu::ShowTipInfomation(bool on)
{
    is_show_tip_widget = on;
}

void DropdownMenu::SetItemSize(int size)
{
    item_size = size;
    for(int i = 0; i < item_vector.size(); i++) item_vector[i]->SetItemSize(size);
}

void DropdownMenu::SetSpace(int size)
{
    item_space = size;
}

void DropdownMenu::CreateAnimation()
{
    anim_show_menu->clear();

    for(int i = 0; i < item_vector.size(); i++)
    {
        QPropertyAnimation* anim = new QPropertyAnimation(item_vector[i], "position", this);
        anim->setEasingCurve(QEasingCurve::OutSine);
        anim->setDuration(300 + (item_vector.size() - i) * 80);

        QPropertyAnimation* opacity = new QPropertyAnimation(item_vector[i], "opacity", this);
        opacity->setDuration(300 + (item_vector.size() - i) * 80);

        anim_show_menu->addAnimation(anim);
        anim_show_menu->addAnimation(opacity);
    }
    this->resize(item_size, this->GetLength());
}

void DropdownMenu::UpdateItemsPosition()
{
    int length = this->GetLength();

    for(int i = 0; i < item_vector.size(); i++)
    {
        QPropertyAnimation* anim = qobject_cast<QPropertyAnimation*>(anim_show_menu->animationAt(i * 2 + 0));

        int y = i * (item_size + item_space);
        anim->setStartValue(QPoint(0, y - length * 0.3 - (item_vector.size() - i) * 100));
        anim->setEndValue(QPoint(0, y));

        QPropertyAnimation* opacity = qobject_cast<QPropertyAnimation*>(anim_show_menu->animationAt(i * 2 + 1));
        opacity->setStartValue(0.0f);
        opacity->setEndValue(1.0f);

        item_vector[i]->SetPosition(QPoint(-1024, -1024));
    }
    this->repaint();
}

int DropdownMenu::GetLength()
{
    return item_vector.size() * (item_size + item_space) - item_space;
}

int DropdownMenu::GetHoverItemIndex(const QPoint& pos)
{
    int index = -1;
    int block = item_size + item_space;

    for(int i = 0; i < item_vector.size(); i++)
    {
        int x = pos.x();
        int y = pos.y();

        if( x >= 0 && x <= item_size && y >= block * i && y <= block * i + item_size ){
            index = i;
            break;
        }
    }
    return index;
}

void DropdownMenu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 1));

    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
    for(int i = 0; i < item_vector.size(); i++)
    {
        item_vector[i]->Paint(painter, is_enter && current_item == i, is_clicked);
    }
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, false);
}

void DropdownMenu::mousePressEvent(QMouseEvent *event)
{
    is_clicked = event->button() & Qt::LeftButton;
}

void DropdownMenu::mouseMoveEvent(QMouseEvent *event)
{
    int index = this->GetHoverItemIndex(event->pos());

    if( current_item != index )
    {
        current_item = index;
        if( current_item != -1 )
        {
            tip_widget->SetText(item_vector[current_item]->GetText());

            int x = this->pos().x();
            int y = this->pos().y() + current_item * (item_size + item_space) + (item_size - tip_widget->height()) * 0.5f;

            if( is_show_tip_widget ) tip_widget->ShowAt(QPoint(x, y));
        }
    }
}

void DropdownMenu::mouseReleaseEvent(QMouseEvent *event)
{
    if( is_clicked )
    {
        int index = this->GetHoverItemIndex(event->pos());
        if( index != -1)
        {
            emit sigClicked(index);
        }
    }
    is_clicked = false;
}

void DropdownMenu::enterEvent(QEvent *)
{
    is_enter = true;
}

void DropdownMenu::leaveEvent(QEvent *)
{
    is_enter = false;
    current_item = -1;
    tip_widget->hide();
    this->HideMenu();
}

void DropdownMenu::focusOutEvent(QFocusEvent *)
{
    if( is_enter == false )
    {
        this->HideMenu();
    }
}
