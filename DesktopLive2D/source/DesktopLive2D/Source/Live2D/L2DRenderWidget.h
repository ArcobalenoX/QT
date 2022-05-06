#ifndef LIVE2DRENDERGLWIDGET_H
#define LIVE2DRENDERGLWIDGET_H

#include <Source/Common.h>
#include <Source/Common/DropdownMenu.h>

#include "L2DModel.h"
#include "L2DRenderer.h"

#include "L2DModelParam.h"
#include "L2DModelController.h"

#include <QMenu>
#include <QTimer>
#include <QWidget>
#include <QVector>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>

/*
** 模型渲染窗口，使用 OpenGL 进行渲染
**/
class L2DRenderGLWidget : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT

    enum MouseAction
    {
        MA_MoveModel,
        MA_ResizeWindow,
        MA_ModelNotice,
        MA_Unknown
    };

public:
    explicit L2DRenderGLWidget(QWidget *parent = 0);

    ~L2DRenderGLWidget();

    void SetModel(L2DModel* model);

    void SetModelUpdateFPS(int fps);

    void BeginGetModelCover();

    QImage GetModelCover();

    void EndGetModelCover();

    DropdownMenu* GetDropdownMenu() { return dropdown_menu; }

    void initializeGL() override;

    void paintGL() override;

    void resizeGL(int width, int height) override;

    void enterEvent(QEvent *event);

    void leaveEvent(QEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

private:
    bool CanResizeWindow(const QPoint& mouse_point);

public slots:
    void slotDefaultModelWindowSize();

private:
    QTimer* timer;
    L2DModel* l2d_model;
    L2DRenderer* l2d_render;

    QOpenGLFramebufferObject* frame_buffer_object;

    DropdownMenu* dropdown_menu;

    QString current_model_name;

    QPoint mouse_origin_point;
    QPoint window_origin_point;
    QSize  window_origin_size;

    MouseAction mouse_action;

    bool is_enter;
    bool do_mouse_move_event;
    bool is_create_model_cover;
};

#endif // LIVE2DRENDERGLWIDGET_H
