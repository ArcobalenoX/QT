#include "CaptureEditer.h"

#include <QPainter>

//#define CE_DEBUG

#define FONT_MARGIN_X       5
#define FONT_MARGIN_Y       2

static const int FontBoxWidth = 85;
static const int FontBoxHeight = 21;


CaptureEditer::CaptureEditer(QWidget *parent) : QWidget(parent)
{
#ifndef CE_DEBUG
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
#endif
    this->setMouseTracking(true);

    capture_state = CS_Uncapture;

    /* 创建右键菜单 */
    menu = new QMenu(this);
    menu->addAction(QString::fromLocal8Bit("确定"), this, SLOT(slotFinishedCapture()));
    menu->addAction(QString::fromLocal8Bit("取消"), this, SLOT(slotCancelCapture()));
    menu->addAction(QString::fromLocal8Bit("重新选择"), this, SLOT(slotReset()));

    btn_ok = new QPushButton(QString::fromLocal8Bit("确定"), this);
    btn_cancel = new QPushButton(QString::fromLocal8Bit("取消"), this);

    connect(btn_ok, SIGNAL(pressed()), this, SLOT(slotFinishedCapture()));
    connect(btn_cancel, SIGNAL(pressed()), this, SLOT(slotCancelCapture()));

    QString styleSheet =
            "QPushButton{ border: 1px solid white; min-height: 20px; min-width: 60px; color: white; }"
            "QPushButton:hover{ border: 1px solid #52BEF7; }";

    btn_ok->setStyleSheet(styleSheet);
    btn_cancel->setStyleSheet(styleSheet);

    /* 字体 */
    font.setFamily(QString::fromLocal8Bit("微软雅黑"));
    font.setPixelSize(12);

    area_selected_box = new AreaSelectedBox(this);

    /* 预览视图 */
    preview_padding_size = 50;
    preview_rect.setRect(preview_padding_size, preview_padding_size, 200, 200);

    this->slotReset();
}

CaptureEditer::~CaptureEditer()
{
    delete area_selected_box;
    qDebug("Delete CaptureEditer");
}

void CaptureEditer::ShowCaptureEditer()
{
    this->slotReset();

#ifdef CE_DEBUG
    this->show();
#else
    this->showFullScreen();
#endif
}

QImage CaptureEditer::GetCaptureImage()
{
    if( selected_image_rect.width() == -1 || selected_image_rect.height() == -1 )
    {
        return background_image;
    }
    else
    {
        return background_image.copy(this->GetCaptureRect());
    }
}

QRect CaptureEditer::GetCaptureRect()
{
    return selected_image_rect;
}

void CaptureEditer::slotReset()
{
    is_start = false;
    is_button_clicked = false;
    capture_state = CS_Uncapture;

    btn_ok->setVisible(false);
    btn_cancel->setVisible(false);

    selected_image_rect.setRect(-1, -1, -1, -1);
    area_selected_box->Reset();
    this->update();
}

void CaptureEditer::SetImage(const QImage& image)
{
    background_image = image;
    this->slotReset();
}

void CaptureEditer::slotFinishedCapture()
{
    emit sigExitCaptureEditer(true);
    this->close();
}

void CaptureEditer::slotCancelCapture()
{
    emit sigExitCaptureEditer(false);
    this->close();
}

void CaptureEditer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(font);

    /* 绘制背景图片 */
    painter.drawImage(this->rect(), background_image);
    if( capture_state != CS_Uncapture )
    {
        painter.fillRect(this->rect(), QColor(0, 0, 0, 180));
    }
    selected_image_rect = area_selected_box->Getselected_rect();

    /* 高亮选择的图片区域 */
    painter.drawImage(selected_image_rect, background_image, selected_image_rect);

    /* 绘制按钮 */
    if( capture_state == CS_Edit )
    {
        int x = selected_image_rect.right() - 135;
        int y = selected_image_rect.bottom() + 5;

        if(this->height() - selected_image_rect.bottom() < 25){
            y = selected_image_rect.bottom() - 25;
        }

        painter.fillRect(QRect(x, y, 130, 20), QColor(0, 0, 0, 200));
        btn_ok->move(x, y);
        btn_cancel->move(x + 70, y);
    }

    /* 绘制十字线 */
    if( capture_state != CS_Edit )
    {
        painter.setPen(Qt::red);
        painter.drawLine(0, mouse_hover_point.y(), this->width(), mouse_hover_point.y());
        painter.drawLine(mouse_hover_point.x(), 0, mouse_hover_point.x(), this->height());

        int y = mouse_hover_point.y() - FontBoxHeight - 2;
        int x = mouse_hover_point.x() + 2;

        if( mouse_hover_point.y() < FontBoxHeight + 20 )
        {
            y = mouse_hover_point.y() + 2 + 25;
        }
        if( mouse_hover_point.x() > this->width() - FontBoxWidth - 20 )
        {
            x = mouse_hover_point.x() - FontBoxWidth - 2;
        }

        /* 绘制光标坐标信息 */
        QRect rect = QRect(x, y, FontBoxWidth, FontBoxHeight);
        painter.setPen(Qt::white);
        painter.fillRect(rect, QColor(0, 0, 0, 160));
        painter.drawText(rect.adjusted(FONT_MARGIN_X, FONT_MARGIN_Y, 0, 0),
                         QString("%1 x %2").arg(mouse_hover_point.x()).arg(mouse_hover_point.y()));
    }

    if( capture_state != CS_Uncapture )
    {
        area_selected_box->Draw(&painter);

        /* 选择区大小信息 */
        int y = selected_image_rect.top() - FontBoxHeight - 2;
        if(selected_image_rect.top() < 25)
        {
            y = selected_image_rect.top() + 2;
        }

        QRect whinfoRect = QRect(selected_image_rect.x() + 2, y, 85, FontBoxHeight);
        painter.setPen(Qt::white);
        painter.fillRect(whinfoRect, QColor(0, 0, 0, 160));
        painter.drawText(whinfoRect.adjusted(FONT_MARGIN_X, FONT_MARGIN_Y, 0, 0),
                         QString("%1 x %2").arg(selected_image_rect.width()).arg(selected_image_rect.height()));
    }
    /* 绘制细节窗口 */
    this->DrawPreview(&painter);
}

void CaptureEditer::DrawPreview(QPainter* painter)
{
    int x = preview_rect.left();
    int y = preview_rect.top();
    int w = preview_rect.width();
    int h = preview_rect.height();

    /* 显示当前像素 RGB */
    QRect rgbRect = QRect(x, y - 25, 127, FontBoxHeight);
    QColor rgb = background_image.pixel(mouse_hover_point.x(), mouse_hover_point.y());

    painter->setPen(Qt::white);
    painter->fillRect(rgbRect, QColor(0, 0, 0, 200));
    painter->drawText(rgbRect.adjusted(FONT_MARGIN_X, FONT_MARGIN_Y, 0, 0),
                      QString("RGB: (%1, %2, %3)").arg(rgb.red()).arg(rgb.green()).arg(rgb.blue()));

    QRect destRect = QRect(x, y, w, h);
    QRect srcRect = QRect(mouse_hover_point.x() - 25, mouse_hover_point.y() - 25, 50, 50);

    painter->fillRect(destRect.adjusted(-2, -2, 3, 3), Qt::black);
    painter->drawImage(destRect, background_image, srcRect.adjusted(1, 1, 1, 1));

    painter->setPen(QPen(QColor(255, 0, 0, 200), 4));
    painter->drawLine(x + 2, y + h / 2 - 2, x + w - 2, y + h / 2 - 2);
    painter->drawLine(x + w / 2 - 2, y + 2, x + w / 2 - 2, y + h - 2);

    painter->setPen(Qt::white);
    painter->drawRect(destRect);
}

void CaptureEditer::mousePressEvent(QMouseEvent *event)
{
    if( event->button() & Qt::LeftButton )
    {
        is_button_clicked = true;
        mouse_origin_point = event->pos();

        area_selected_box->BeginSelectedBox(event->pos());

        /* 鼠标点击后，进入选择区域创建状态（编辑选择框） */
        if( capture_state == CS_Uncapture )
        {
            capture_state =  CS_Capturing;
        }
    }
}

void CaptureEditer::mouseMoveEvent(QMouseEvent *event)
{
    area_selected_box->UpdateSelectedBox(event->pos());
    mouse_hover_point = event->pos();

    /* 预览窗口的显示位置 */
    bool displayTL = true;
    displayTL &= mouse_hover_point.x() >= preview_rect.left() - 50;
    displayTL &= mouse_hover_point.y() >= preview_rect.top() - 50;

    bool displayBR = true;
    displayBR &= mouse_hover_point.x() <= preview_rect.right() + 50;
    displayBR &= mouse_hover_point.y() <= preview_rect.bottom() + 50;

    if( preview_rect.left() < this->width() / 2 && displayBR )
    {
        int x = this->width() - preview_rect.width() - preview_padding_size;
        int y = this->height() - preview_rect.height() - preview_padding_size;
        preview_rect.moveTopLeft(QPoint(x, y));
    }
    else if(displayTL)
    {
        preview_rect.moveTopLeft(QPoint(preview_padding_size, preview_padding_size));
    }

    this->update();
}

void CaptureEditer::mouseReleaseEvent(QMouseEvent *event)
{
    if( event->button() & Qt::LeftButton )
    {
        area_selected_box->EndSelectedBox();

        /* 创建选择区域后，进入选择区域编辑状态 */
        if( capture_state == CS_Capturing )
        {
            capture_state  = CS_Edit;

            btn_ok->setVisible(true);
            btn_cancel->setVisible(true);
        }
    }

    if( event->button() & Qt::RightButton )
    {
        menu->exec(QCursor::pos());
    }
    is_button_clicked = false;
}
