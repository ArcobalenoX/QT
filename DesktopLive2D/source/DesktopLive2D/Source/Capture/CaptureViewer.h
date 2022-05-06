#ifndef CAPTUREVIEWER_H
#define CAPTUREVIEWER_H

#include <QPen>
#include <QImage>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>

#include <Source/Common/DropdownMenu.h>

class CaptureViewer : public QWidget
{
    Q_OBJECT

    enum MouseAction
    {
        MA_MoveWindow,
        MA_ResizeWindow,
        MA_Count
    };

public:
    explicit CaptureViewer(QWidget *parent = 0);

    ~CaptureViewer();

    void SetImage(const QImage& image);

    void Show(const QRect& rect);

    void SetBlock(bool on);

    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void enterEvent(QEvent *);

    void leaveEvent(QEvent *);

private:
    bool SaveImage();

    bool CanResizeWindow(const QPoint& pos);

signals:
    void sigClose(CaptureViewer* object);

public slots:
    void slotDropdownMenuClicked(int index);

private:
    DropdownMenu* dropdown_menu;

    MouseAction mouse_action;

    QImage image;
    QImage icon_resize;

    QRect icon_rect;
    QRect window_origin_rect;

    QSize window_origin_size;

    QPoint mouse_origin_point;
    QPoint window_origin_point;

    bool is_move;
    bool is_block;
    bool is_enter;
    bool is_resize_window;
};

#endif // CAPTUREVIEWER_H
