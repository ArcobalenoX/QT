#include "AreaSelectedBox.h"
#include <QWidget>

AreaSelectedBox::AreaSelectedBox(QWidget *widget)
{
    this->widget = widget;
    control_point_size = 6;

    this->Reset();
}

void AreaSelectedBox::Reset()
{
    is_pressed = false;
    is_edit_area = false;
    is_drawable = false;

    selected_rect.setRect(-1, -1, -1, -1);
}

void AreaSelectedBox::UpdateCursorShape(const QPoint& pos)
{
    if( selected_rect.contains(pos) ){
        widget->setCursor(is_pressed ? Qt::ClosedHandCursor : Qt::OpenHandCursor);
    }
    else if( hitTLRect.contains(pos) || hitBRRect.contains(pos) ){
        widget->setCursor(Qt::SizeFDiagCursor);
    }
    else if( hitTRRect.contains(pos) || hitBLRect.contains(pos) ){
        widget->setCursor(Qt::SizeBDiagCursor);
    }
    else if( hitCLRect.contains(pos) || hitCRRect.contains(pos) ){
        widget->setCursor(Qt::SizeHorCursor);
    }
    else if( hitTCRect.contains(pos) || hitBCRect.contains(pos) ){
        widget->setCursor(Qt::SizeVerCursor);
    }
    else{
        widget->setCursor(Qt::ArrowCursor);
    }
}

void AreaSelectedBox::SetMoveAction(const QPoint& pos)
{
    /* 判断鼠标拖动时对选择框进行的操作（编辑控制点、移动选择框） */
    mouse_action = MA_MOVE_NONE;

    if(tlRect.contains(pos)){
        mouse_action = MA_MOVE_TL;
    }
    else if(hitTCRect.contains(pos)){
        mouse_action = MA_MOVE_TC;
    }
    else if(hitTRRect.contains(pos)){
        mouse_action = MA_MOVE_TR;
    }
    else if(hitCLRect.contains(pos)){
        mouse_action = MA_MOVE_CL;
    }
    else if(hitCRRect.contains(pos)){
        mouse_action = MA_MOVE_CR;
    }
    else if(hitBLRect.contains(pos)){
        mouse_action = MA_MOVE_BL;
    }
    else if(hitBCRect.contains(pos)){
        mouse_action = MA_MOVE_BC;
    }
    else if(hitBRRect.contains(pos)){
        mouse_action = MA_MOVE_BR;
    }
    else if(selected_rect.contains(pos)){
        mouse_action = MA_MOVE_RECT;
    }
}

void AreaSelectedBox::UpdateControlPoints()
{
    /* 选择区域改变，重新计算控制点的位置 */
    int x1 = selected_rect.left();
    int y1 = selected_rect.top();
    int x2 = selected_rect.right();
    int y2 = selected_rect.bottom();

    int wc = (x2 + x1) / 2;
    int hc = (y2 + y1) / 2;

    /* 控制点的绘制位置 */
    tlRect.setRect(x1 - 2, y1 - 2, control_point_size, control_point_size);
    tcRect.setRect(wc - 2, y1 - 2, control_point_size, control_point_size);
    trRect.setRect(x2 - 2, y1 - 2, control_point_size, control_point_size);

    clRect.setRect(x1 - 2, hc - 2, control_point_size, control_point_size);
    crRect.setRect(x2 - 2, hc - 2, control_point_size, control_point_size);

    blRect.setRect(x1 - 2, y2 - 2, control_point_size, control_point_size);
    bcRect.setRect(wc - 2, y2 - 2, control_point_size, control_point_size);
    brRect.setRect(x2 - 2, y2 - 2, control_point_size, control_point_size);

    /* 放大控制点的碰撞检测面积，易于选择控制点 */
    int padding = 4;
    hitTLRect = tlRect.adjusted(-padding, -padding, padding, padding);
    hitTCRect = tcRect.adjusted(-padding, -padding, padding, padding);
    hitTRRect = trRect.adjusted(-padding, -padding, padding, padding);

    hitCLRect = clRect.adjusted(-padding, -padding, padding, padding);
    hitCRRect = crRect.adjusted(-padding, -padding, padding, padding);

    hitBLRect = blRect.adjusted(-padding, -padding, padding, padding);
    hitBCRect = bcRect.adjusted(-padding, -padding, padding, padding);
    hitBRRect = brRect.adjusted(-padding, -padding, padding, padding);
}

QRect AreaSelectedBox::Getselected_rect()
{
    QRect rect = selected_rect;

    /* 确保 QRect 右下角位置永远大于左上角的位置 */
    int x = qMin(rect.left(), rect.right());
    int y = qMin(rect.bottom(), rect.top());
    int w = qAbs(rect.left() - rect.right());
    int h = qAbs(rect.bottom() - rect.top());
    rect.setRect(x, y, w, h);

    return rect;
}

void AreaSelectedBox::Draw(QPainter* painter)
{
    if( is_drawable == false ) return;

    int x1 = selected_rect.left();
    int y1 = selected_rect.top();
    int x2 = selected_rect.right();
    int y2 = selected_rect.bottom();

    x1 += 1;
    y1 += 1;
    x2 += 1;
    y2 += 1;

    /* 绘制编辑框边界 */
    QColor color = QColor(0, 0, 255);
    painter->setPen(QPen(color, 2));
    painter->drawLine(x1, y1, x2, y1);
    painter->drawLine(x1, y2, x2, y2);
    painter->drawLine(x1, y1, x1, y2);
    painter->drawLine(x2, y1, x2, y2);

    /* 绘制编辑点 */
    painter->fillRect(tlRect, color);
    painter->fillRect(tcRect, color);
    painter->fillRect(trRect, color);

    painter->fillRect(clRect, color);
    painter->fillRect(crRect, color);

    painter->fillRect(blRect, color);
    painter->fillRect(bcRect, color);
    painter->fillRect(brRect, color);
}

void AreaSelectedBox::BeginSelectedBox(const QPoint& pos)
{
    is_move = false;
    is_pressed = true;
    origin_point = pos;

    if( is_edit_area ){
        origin_selected_rect = selected_rect;
        this->SetMoveAction(pos);
    }

    this->UpdateCursorShape(pos);
    widget->update();
}

void AreaSelectedBox::UpdateSelectedBox(const QPoint& pos)
{
    if( is_pressed == false ){
        this->UpdateCursorShape(pos);
        return;
    }
    is_move = true;

    if( is_drawable == false ) is_drawable = true;

    if( is_edit_area == false ){
        int x = qMin(origin_point.x(),  pos.x());
        int y = qMin(origin_point.y(),  pos.y());
        int w = qAbs(origin_point.x() - pos.x());
        int h = qAbs(origin_point.y() - pos.y());

        selected_rect.setRect(x, y, w, h);
        this->UpdateControlPoints();
        return;
    }

    /* 移动控制点 */
    QPoint offset = pos - origin_point;

    switch (mouse_action) {
    case MA_MOVE_RECT:{
        int offsetx = qMax(0, qMin(widget->width() - selected_rect.width(), origin_selected_rect.left() + offset.x()));
        int offsety = qMax(0, qMin(widget->height() - selected_rect.height(), origin_selected_rect.top() + offset.y()));
        selected_rect.moveTopLeft(QPoint(offsetx, offsety));
    }
        break;
    case MA_MOVE_TL:
        selected_rect.setTopLeft(origin_selected_rect.topLeft() + offset);
        break;
    case MA_MOVE_TC:
        selected_rect.setTop(origin_selected_rect.top() + offset.y());
        break;
    case MA_MOVE_TR:
        selected_rect.setTopRight(origin_selected_rect.topRight() + offset);
        break;
    case MA_MOVE_CL:
        selected_rect.setLeft(origin_selected_rect.left() + offset.x());
        break;
    case MA_MOVE_CR:
        selected_rect.setRight(origin_selected_rect.right() + offset.x());
        break;
    case MA_MOVE_BL:
        selected_rect.setBottomLeft(origin_selected_rect.bottomLeft() + offset);
        break;
    case MA_MOVE_BC:
        selected_rect.setBottom(origin_selected_rect.bottom() + offset.y());
        break;
    case MA_MOVE_BR:
        selected_rect.setBottomRight(origin_selected_rect.bottomRight() + offset);
        break;
    }
    this->UpdateControlPoints();
}

void AreaSelectedBox::EndSelectedBox()
{
    is_pressed = false;

    if( is_edit_area == false ){
        is_edit_area = true;
    }
    widget->update();
}
