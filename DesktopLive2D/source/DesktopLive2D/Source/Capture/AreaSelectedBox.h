#ifndef AREASELECTEDBOX_H
#define AREASELECTEDBOX_H

#include <QObject>
#include <QPainter>

class AreaSelectedBox : public QObject
{
    Q_OBJECT

    enum MouseAction
    {
        MA_MOVE_NONE,
        MA_MOVE_RECT,
        MA_MOVE_TL,
        MA_MOVE_TC,
        MA_MOVE_TR,
        MA_MOVE_CL,
        MA_MOVE_CR,
        MA_MOVE_BL,
        MA_MOVE_BC,
        MA_MOVE_BR,
    };

public:
    explicit AreaSelectedBox(QWidget *widget);

    void Reset();

    QRect Getselected_rect();

    void Draw(QPainter* painter);

    void BeginSelectedBox(const QPoint& pos);

    void UpdateSelectedBox(const QPoint& pos);

    void EndSelectedBox();

private:
    void UpdateCursorShape(const QPoint& pos);

    void UpdateControlPoints();

    void SetMoveAction(const QPoint& pos);

private:
    QWidget* widget;

    QPoint origin_point;
    MouseAction mouse_action;

    bool is_move;
    bool is_pressed;
    bool is_drawable;
    bool is_edit_area;

    int control_point_size;

    QRect selected_rect;
    QRect origin_selected_rect;

    QRect tlRect;
    QRect tcRect;
    QRect trRect;
    QRect clRect;
    QRect crRect;
    QRect blRect;
    QRect bcRect;
    QRect brRect;

    QRect hitTLRect;
    QRect hitTCRect;
    QRect hitTRRect;
    QRect hitCLRect;
    QRect hitCRRect;
    QRect hitBLRect;
    QRect hitBCRect;
    QRect hitBRRect;
};

#endif // AREASELECTEDBOX_H
