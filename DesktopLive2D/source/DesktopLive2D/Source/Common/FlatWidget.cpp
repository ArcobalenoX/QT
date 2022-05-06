#include "FlatWidget.h"
#include "Debug.h"
#include "PathHelper.h"

#include <QFile>
#include <QPainter>
#include <QPushButton>

FlatWidget::FlatWidget(QWidget *parent) : QWidget(parent), center_widget(nullptr)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

    mouse_action = MA_Unknown;
    padding = 8;
    head_height = 30;
    is_resizable = false;
    is_delete_after_close = false;

    this->LoadStyleSheet(PathHelper::GetStyleSheetPath() + "style.css");

    this->resize(400, 300);
    this->Init();
}

void FlatWidget::LoadStyleSheet(const QString& path)
{
    QFile file(path);
    if( file.open(QFile::ReadOnly) )
    {
        this->setStyleSheet(file.readAll());
        file.close();
    }
    else
    {
        Debug::Printf(QString::fromLocal8Bit("不能加载格式文件"));
    }
}

void FlatWidget::Init()
{
    center_widget = new QWidget(this);
    center_widget->move(padding, head_height + padding);
    center_widget->resize(this->width() - padding * 2, this->height() - center_widget->y() - padding);

    /* 关闭窗口按钮 */
    btn_close = new QPushButton(this);
    connect(btn_close, SIGNAL(pressed()), this, SLOT(slotCloseButtonClicked()));

    btn_close->resize(20, 20);
    btn_close->move(this->width() - 20 - 10, 5);
    btn_close->setStyleSheet(
                "QPushButton{ border-radius: 10px; background-color: #2ecc71; }"
                "QPushButton:hover{ background-color: #27ae60; image: url(:/Resources/Icons/icon_close.png); }"
                );

    title = "";
}

void FlatWidget::SetWindowTitle(const QString& title)
{
    this->title = title;
}

void FlatWidget::SetDeleteAfterClose(bool on)
{
    is_delete_after_close = on;
}

void FlatWidget::SetLayout(QLayout* layout, int margin)
{
    layout->setMargin(margin);
    center_widget->setLayout(layout);
}

void FlatWidget::resize(int w, int h)
{
    w = qMax(200, w);
    h = qMax(100, h);

    QWidget::resize(w, h);

    if( center_widget )
    {
        center_widget->move(padding, head_height + padding);
        center_widget->resize(w - padding * 2, h - center_widget->y() - padding);
    }
}

void FlatWidget::resize(const QSize& size)
{
    this->resize(size.width(), size.height());
}

void FlatWidget::resizeEvent(QResizeEvent *event)
{
    btn_close->move(this->width() - 20 - 10, 5);

    center_widget->move(padding, head_height + padding);
    center_widget->resize(this->width() - padding * 2, this->height() - center_widget->y() - padding);

    QWidget::resizeEvent(event);
}

void FlatWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(52, 73, 94, 235));

    QRect border_rect(0, 0, this->width(), head_height);
    painter.fillRect(border_rect, QColor(44, 62, 80, 210));
    painter.drawText(border_rect.adjusted(15, 3, -5,-2), this->title);

    painter.setPen(QColor("#2ecc71"));
    painter.drawLine(0, border_rect.height(), this->width(), border_rect.height());

    QWidget::paintEvent(event);
}

void FlatWidget::mousePressEvent(QMouseEvent *event)
{
    mouse_origin_point  = event->globalPos();
    window_origin_point = this->pos();
    window_origin_size  = this->size();

    if( event->button() & Qt::LeftButton )
    {
        if( event->y() <= head_height )
        {
            mouse_action = MA_MoveWindow;
        }
        else if( this->width() - event->pos().x() <= padding && this->height() - event->pos().y() <= padding )
        {
            mouse_action = MA_Resize;
        }
        else
        {
            mouse_action = MA_Unknown;
        }
    }
    QWidget::mousePressEvent(event);
}

void FlatWidget::mouseMoveEvent(QMouseEvent *event)
{
    if( is_resizable && mouse_action == MA_Unknown )
    {
        bool resize = this->width() - event->pos().x() <= padding && this->height() - event->pos().y() <= padding;
        this->setCursor(resize ? Qt::SizeFDiagCursor : Qt::ArrowCursor);
    }

    if( event->buttons() & Qt::LeftButton )
    {
        QPoint offset = event->globalPos() - mouse_origin_point;
        if( mouse_action == MA_MoveWindow )
        {
            this->move(window_origin_point + offset);
        }
        else if( is_resizable && mouse_action == MA_Resize )
        {
            this->resize(window_origin_size.width() + offset.x(), window_origin_size.height() + offset.y());
        }
    }
    QWidget::mouseMoveEvent(event);
}

void FlatWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_action = MA_Unknown;
    QWidget::mouseReleaseEvent(event);
}

void FlatWidget::slotCloseButtonClicked()
{
    emit sigCloseButtonClicked();

    if( is_delete_after_close )
    {
        this->deleteLater();
    }
    else
    {
        this->close();
    }
}
