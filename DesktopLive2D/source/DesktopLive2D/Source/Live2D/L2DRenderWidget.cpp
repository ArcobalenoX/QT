#include "L2DRenderWidget.h"
#include "L2DModel.h"

#include <QApplication>

#include "../External/Live2D/Live2D.h"

#include <Source/Common/Debug.h>
#include <Source/Common/DataManager.h>
#include <Source/Common/StyleSheetManager.h>
#include <Source/Common/PathHelper.h>

#define MODEL_FACE_SIZE 300

L2DRenderGLWidget::L2DRenderGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , l2d_render(nullptr)
    , l2d_model(nullptr)
    , timer(nullptr)
    , is_create_model_cover(false)
    , is_enter(false)
{
    if( Global::UserData.WindowStaysOnTop ){
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    }
    else{
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    }

    this->setAttribute(Qt::WA_TranslucentBackground);

    /* 由于使用了 Qt::Tool，故设置这个属性 Qt::WA_QuitOnClose 使得这个 widget 关闭时退出程序 */
    this->setAttribute(Qt::WA_QuitOnClose);
    this->setMouseTracking(true);

    l2d_render = new L2DRenderer();
    mouse_action = MA_Unknown;

    timer = new QTimer(this);
    timer->setInterval(1000 / 48);
    timer->start();

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    QString root = PathHelper::GetResourcePath();

    dropdown_menu = new DropdownMenu;
    dropdown_menu->AddItem(root + "Icons/icon_capture_32.png", QString::fromLocal8Bit("截屏工具"));
    dropdown_menu->AddItem(root + "Icons/icon_music_player_32.png", QString::fromLocal8Bit("音乐播放器"));
    dropdown_menu->AddItem(root + "Icons/icon_setting_32.png", QString::fromLocal8Bit("设置"));
    dropdown_menu->AddItem(root + "Icons/icon_quit_32.png", QString::fromLocal8Bit("退出程序"));
}

L2DRenderGLWidget::~L2DRenderGLWidget()
{
    delete l2d_render;
    delete dropdown_menu;
}

void L2DRenderGLWidget::SetModel(L2DModel* model)
{
    l2d_model = model;
    l2d_render->SetModel(l2d_model);
    l2d_render->SetWindowSize(this->width(), this->height());
}

void L2DRenderGLWidget::SetModelUpdateFPS(int fps)
{
    timer->setInterval(1000 / fps);
}

void L2DRenderGLWidget::BeginGetModelCover()
{
    /* 渲染模型到当前帧缓冲对象 */
    frame_buffer_object = new QOpenGLFramebufferObject(MODEL_FACE_SIZE, MODEL_FACE_SIZE);
    frame_buffer_object->bind();

    window_origin_size = this->size();
    this->resize(MODEL_FACE_SIZE, MODEL_FACE_SIZE);

    is_create_model_cover = true;
}

QImage L2DRenderGLWidget::GetModelCover()
{
    return frame_buffer_object->toImage();
}

void L2DRenderGLWidget::EndGetModelCover()
{
    /* 绑定回原来的帧缓冲对象 */
    frame_buffer_object->bindDefault();
    delete frame_buffer_object;

    this->resize(window_origin_size);
    is_create_model_cover = false;

    this->makeCurrent();
}

void L2DRenderGLWidget::initializeGL()
{
    this->initializeOpenGLFunctions();
}

void L2DRenderGLWidget::resizeGL(int width, int height)
{
    l2d_render->SetWindowSize(width, height);
}

void L2DRenderGLWidget::paintGL()
{
    QColor color(0, 0, 0, 0);

    if( Global::UserData.DisplayBackground && is_create_model_cover == false )
    {
        if( Global::UserData.HoverBackground == false || is_enter )
        {
            color = Global::UserData.BackgroundColor;
        }
    }
    glClearColor(color.redF(),  color.greenF(), color.blueF(), color.alphaF());
    glClear(GL_COLOR_BUFFER_BIT);

    /* 渲染模型 */
    l2d_render->Draw();
}

bool L2DRenderGLWidget::CanResizeWindow(const QPoint& mouse_point)
{
    static int resize_area_size = 32;

    bool can_resize_window = true;
    can_resize_window &= mouse_point.x() >= width()  - resize_area_size;
    can_resize_window &= mouse_point.y() >= height() - resize_area_size;

    return can_resize_window;
}

void L2DRenderGLWidget::enterEvent(QEvent *event)
{
    is_enter = true;
}

void L2DRenderGLWidget::leaveEvent(QEvent *event)
{
    is_enter = false;
}

void L2DRenderGLWidget::mousePressEvent(QMouseEvent *event)
{
    do_mouse_move_event = false;
    QPoint mouse_point = event->pos();

    if( event->button() & Qt::LeftButton )
    {
        mouse_action = MA_ModelNotice;
        l2d_render->MousePress(mouse_point.x(), mouse_point.y());
    }
    else if( event->button() & Qt::RightButton )
    {
        bool resize_window = this->CanResizeWindow(mouse_point) && Global::UserData.ResizeModelWindow;

        mouse_action = resize_window ? MA_ResizeWindow : MA_MoveModel;
        this->setCursor(resize_window ? Qt::SizeFDiagCursor : Qt::SizeAllCursor);
    }

    mouse_origin_point  = event->globalPos();
    window_origin_point = this->pos();
    window_origin_size  = this->size();
}

void L2DRenderGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if( do_mouse_move_event == false && event->button() == Qt::RightButton)
    {
        dropdown_menu->ShowMenu(QPoint(this->pos().x(), this->pos().y()));
    }

    if( do_mouse_move_event )
    {
        if( mouse_action == MA_MoveModel )
        {
            Global::UserData.ModelWindowPos = this->pos();
        }
        else if( mouse_action == MA_ResizeWindow )
        {
            Global::UserData.ModelWindowSize = this->size();
        }
        DataManager::GetInstance()->Update();
    }
    mouse_action = MA_Unknown;
}

void L2DRenderGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    do_mouse_move_event = true;
    QPoint mouse_point = event->pos();

    if( mouse_action == MA_Unknown )
    {
        bool resizable = this->CanResizeWindow(mouse_point) && Global::UserData.ResizeModelWindow;
        this->setCursor(resizable ? Qt::SizeFDiagCursor : Qt::ArrowCursor);
    }
    else if( mouse_action == MA_MoveModel && event->buttons() == Qt::RightButton )
    {
        QPoint offset = event->globalPos() - mouse_origin_point;
        this->move(window_origin_point + offset);
    }
    else if( mouse_action == MA_ResizeWindow && event->buttons() == Qt::RightButton )
    {
        QPoint offset = event->globalPos() - mouse_origin_point;
        this->resize(window_origin_size.width() + offset.x(), window_origin_size.height() + offset.y());
        this->repaint();
    }
    else if( mouse_action == MA_ModelNotice && event->buttons() == Qt::LeftButton)
    {
        l2d_render->MouseDrag(mouse_point.x(), mouse_point.y());
    }
}

void L2DRenderGLWidget::wheelEvent(QWheelEvent *event)
{
    //pL2DRenderer->MouseWheel(event->delta(), event->x(), event->y());
}

void L2DRenderGLWidget::slotDefaultModelWindowSize()
{
    Global::UserData.ModelWindowSize = QSize(MODEL_WINDOW_SIZE, MODEL_WINDOW_SIZE);
    this->resize(Global::UserData.ModelWindowSize);
    DataManager::GetInstance()->Update();
}
