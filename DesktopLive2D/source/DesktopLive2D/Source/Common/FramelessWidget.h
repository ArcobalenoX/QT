#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include <QWidget>
#include <QMouseEvent>

class FramelessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FramelessWidget(QWidget *parent = 0);

    void SetMoveWidgetButton(Qt::MouseButton button);

    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

protected:
    QPoint mouse_origin_point;
    QPoint window_origin_point;

    Qt::MouseButton moveButton;
};

#endif // FRAMELESSWIDGET_H
