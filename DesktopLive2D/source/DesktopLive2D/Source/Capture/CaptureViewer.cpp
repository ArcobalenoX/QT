#include "CaptureViewer.h"

#include <QDir>
#include <QPainter>
#include <QDateTime>
#include <QFileDialog>

#include <Source/Common/PathHelper.h>

static const int IconFixed  = 0;
static const int IconScale  = 1;
static const int IconSave   = 2;
static const int IconClose  = 3;

static const int BlockSize  = 56;

static QColor IconColor = QColor(46, 204, 113, 200);

CaptureViewer::CaptureViewer(QWidget *parent)
    : QWidget(parent)
    , is_move(false)
    , is_block(false)
    , is_enter(false)
    , is_resize_window(false)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setMouseTracking(true);

    mouse_action = MA_Count;

    QString root = PathHelper::GetResourcePath();

    dropdown_menu = new DropdownMenu;
    dropdown_menu->AddItem(root + "Icons/icon_fixed_32.png");
    dropdown_menu->AddItem(root + "Icons/icon_scale11_32.png");
    dropdown_menu->AddItem(root + "Icons/icon_save_32.png");
    dropdown_menu->AddItem(root + "Icons/icon_sub_32.png");
    dropdown_menu->ShowTipInfomation(false);

    icon_resize.load(root + "Icons/icon_fixed_32.png");

    connect(dropdown_menu, SIGNAL(sigClicked(int)), this, SLOT(slotDropdownMenuClicked(int)));
}

CaptureViewer::~CaptureViewer()
{
    delete dropdown_menu;
    qDebug("Delete CaptureViewer");
}

void CaptureViewer::SetImage(const QImage& image)
{
    this->image = image;
}

void CaptureViewer::Show(const QRect& rect)
{
    /* 留有 1 像素的边框做突出效果，将视图和背景区分开 */
    this->move(rect.topLeft() - QPoint(1, 1));
    this->resize(rect.size() + QSize(2, 2));
    this->show();
}

void CaptureViewer::SetBlock(bool on)
{
    /* 暂时不需要使用视图时，最小化为方块显示 */
    if( is_block == on ) return;

    is_block = on;
    QRect rect;

    if( is_block )
    {
        window_origin_rect.setRect(this->pos().x(), this->pos().y(), this->width(), this->height());

        int x = this->pos().x() + (this->width() - BlockSize) * 0.5f;
        int y = this->pos().y() + (this->height() - BlockSize) * 0.5f;
        rect.setRect(x, y, BlockSize, BlockSize);
    }
    else
    {
        int x = this->pos().x() - (window_origin_rect.width() - BlockSize) * 0.5f;
        int y = this->pos().y() - (window_origin_rect.height() - BlockSize) * 0.5f;
        rect.setRect(x, y, window_origin_rect.width(), window_origin_rect.height());
    }
    this->move(rect.topLeft());
    this->resize(rect.size());
}

bool CaptureViewer::SaveImage()
{
    /* 将图像保存为 png 格式质量最好 */
    QString caption = QString::fromLocal8Bit("保存截图图片");
    QString filter = "Images (*.png)";

    /* 默认以当前时间为保存的图像文件名 */
    QString dir_path = QDir::currentPath() + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd_hhmmss.png");
    QString file_path = QFileDialog::getSaveFileName(this, caption, dir_path, filter);

    if( file_path.isNull() == false )
    {
        image.save(file_path, 0, 100);
        return true;
    }
    return false;
}

bool CaptureViewer::CanResizeWindow(const QPoint& pos)
{
    int size = 28;
    bool resize = true;

    resize &= is_resize_window;
    resize &= (this->width() - pos.x() <= size);
    resize &= (this->height() - pos.y() <= size);

    return resize;
}

void CaptureViewer::slotDropdownMenuClicked(int index)
{
    if( index == IconFixed && is_block == false )
    {
        is_resize_window = !is_resize_window;
        this->update();
    }
    else if( index == IconScale && is_block == false )
    {
        this->resize(image.size() + QSize(2, 2));
        this->update();
    }
    else if( index == IconSave )
    {
        if( this->SaveImage() ){
            emit sigClose(this);
            this->deleteLater();
        }
    }
    else if( index == IconClose )
    {
        emit sigClose(this);
        this->deleteLater();
    }
}

void CaptureViewer::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    /* 移动视图时的半透明效果 */
    painter.setOpacity((mouse_action == MA_MoveWindow && is_move) ? 0.5f : 1.0f);

    painter.fillRect(this->rect(), Qt::black);

    if( image.isNull() == false ){
        painter.drawImage(this->rect().adjusted(1, 1, -1, -1), image);
    }

    painter.setPen(Qt::white);
    painter.drawRect(this->rect());

    bool enough_size = (image.width() >= BlockSize && image.height() >= BlockSize);
    if( enough_size && is_enter && !is_block && !is_resize_window )
    {
        icon_rect.setRect(this->width() - 35, this->height() - 35, 28, 28);
        QPainterPath circle;
        circle.addEllipse(icon_rect);

        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter.setPen(Qt::NoPen);
        painter.setBrush(IconColor);
        painter.drawPath(circle);
        painter.drawImage(icon_rect.adjusted(4, 4, -4, -4), icon_resize);
    }
}

void CaptureViewer::mousePressEvent(QMouseEvent *event)
{
    /* 记录坐标信息，在 mouseMoveEvent 中进行视图的移动和缩放计算 */
    if( event->button() & Qt::LeftButton )
    {
        window_origin_point = this->pos();
        mouse_origin_point = event->globalPos();
        window_origin_size = this->size();

        /* 通过鼠标点击时在视图中的位置，判断 mouseMoveEvent 中拖动鼠标时执行的操作 */
        mouse_action = (this->CanResizeWindow(event->pos()) && is_block == false) ? MA_ResizeWindow : MA_MoveWindow;

        is_move = false;
        this->update();
    }
}

void CaptureViewer::mouseMoveEvent(QMouseEvent *event)
{
    if( event->buttons() & Qt::LeftButton )
    {
        is_move = true;

        if( mouse_action == MA_MoveWindow )
        {
            this->move(window_origin_point + event->globalPos() - mouse_origin_point);
        }
        else if( mouse_action == MA_ResizeWindow && is_block == false )
        {
            QPoint offset = event->globalPos() - mouse_origin_point;
            int new_width = window_origin_size.width() + offset.x();
            int new_height = (float)new_width / window_origin_size.width() * window_origin_size.height();
            this->resize(new_width, new_height);
        }
        this->update();
        return;
    }

    if( is_block == false )
    {
        this->setCursor(this->CanResizeWindow(event->pos()) ? Qt::SizeFDiagCursor : Qt::ArrowCursor);
    }
}

void CaptureViewer::mouseReleaseEvent(QMouseEvent *event)
{
    /* 鼠标右键点击，显示下拉菜单 */
    if( event->button() & Qt::RightButton && is_move == false )
    {
        dropdown_menu->ShowMenu(this->pos() + QPoint(this->width() + dropdown_menu->width() + 10, 0));
    }
    this->update();

    is_move = false;
    mouse_action = MA_Count;
}

void CaptureViewer::mouseDoubleClickEvent(QMouseEvent *event)
{
    /* 鼠标左键双击，使视图在最小化和最大化间切换 */
    if( event->button() & Qt::LeftButton )
    {
        this->SetBlock(!is_block);
    }
}

void CaptureViewer::enterEvent(QEvent *)
{
    is_enter = true;
    this->update();
}

void CaptureViewer::leaveEvent(QEvent *)
{
    is_enter = false;
    this->update();
}
