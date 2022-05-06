/**************************************************
** 功能：
** 1、使 QWidget 能够在靠近屏幕边缘时自动隐藏大部分的窗体
***************************************************/

#ifndef HIDEINSIDE_H
#define HIDEINSIDE_H

#include <QEvent>
#include <QObject>
#include <QWidget>
#include <QTimeLine>

class HideInSide : public QObject
{
    Q_OBJECT

    enum Direction
    {
        Left, Right, Top, Bottom, None
    } direction;

public:
    explicit HideInSide(QWidget *obj, QObject *parent = 0);

    virtual ~HideInSide();

    void Enter();

    void Leave();

    void SetDuration(int value) { timeLine.setDuration(value); }

    void SetDiaplayRatio(qreal value) { display_ratio = value; }

    void SetEnable(bool enable) { this->is_enable = enable; }

    bool IsHide() { return !is_show; }

protected:
    bool eventFilter(QObject *, QEvent *event);

private:
    void Show();

    void Hide();

    Direction GetCurrentDirection();

    void StartAnimation();

    void CalcHideValue();

    void CalcShowValue();

public slots:
    void slotStep(int value);

private:
    QTimeLine timeLine;
    QWidget *widget;

    float display_ratio;

    bool is_enable;
    bool is_show;
    bool is_finished;

    QSize screen_size;

    QPoint begin_point;
    QPoint end_point;
    QPoint current_point;
};

#endif // HIDEINSIDE_H
