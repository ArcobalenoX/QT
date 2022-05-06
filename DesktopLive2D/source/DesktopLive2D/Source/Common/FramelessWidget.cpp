#include "FramelessWidget.h"

#include <QPainter>

FramelessWidget::FramelessWidget(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    this->SetMoveWidgetButton(Qt::LeftButton);
}

void FramelessWidget::SetMoveWidgetButton(Qt::MouseButton button)
{
    moveButton = button;
}

void FramelessWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(0, 0, 0, 200));
}

void FramelessWidget::mousePressEvent(QMouseEvent *event)
{
    if( event->button() & moveButton ){
        mouse_origin_point = event->globalPos();
        window_origin_point = this->pos();
    }
}

void FramelessWidget::mouseMoveEvent(QMouseEvent *event)
{
    if( event->buttons() & moveButton ){
        this->move(window_origin_point + event->globalPos() - mouse_origin_point);
    }
}

