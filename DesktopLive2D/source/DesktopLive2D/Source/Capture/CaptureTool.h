#ifndef CAPTURETOOL_H
#define CAPTURETOOL_H


#include <QMenu>
#include <QImage>
#include <QWidget>
#include <QVector>
#include <QMouseEvent>
#include <QPaintEvent>

#include "CaptureEditer.h"
#include "CaptureViewer.h"

#include <Source/Common/HideInSide.h>

class CaptureTool : public QWidget
{
    Q_OBJECT
public:
    explicit CaptureTool(QWidget *parent = 0);

    ~CaptureTool();

    void Capture();

protected:
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void enterEvent(QEvent *);

    void leaveEvent(QEvent *);

public slots:
    void slotProcessCaptureEditer(bool succeed);

    void slotCaptureViewerClose(CaptureViewer* object);

    void slotCaptureContainViewer(bool on);

    void slotMinimizeAllViewer();

    void slotMaximizeAllViewer();

    void slotClearAllViewer();

private:
    QPoint mouse_origin_point;
    QPoint window_origin_point;

    HideInSide* hide_in_side;
    CaptureEditer* capture_editer;
    QVector<CaptureViewer*>  capture_viewers;

    QMenu* menu;
    QAction* act_contain_viewer;

    bool is_enter;
    bool is_move_window;
    bool is_button_clicked;
    bool is_capture_contain_viewer;

    QImage icon_normal;
    QImage icon_active;
};

#endif // CAPTURETOOL_H
