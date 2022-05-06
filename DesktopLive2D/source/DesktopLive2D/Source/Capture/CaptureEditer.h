#ifndef CAPTUREEDITER_H
#define CAPTUREEDITER_H

#include <QWidget>
#include <QImage>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QMenu>
#include <QPushButton>

#include "AreaSelectedBox.h"

class CaptureEditer : public QWidget
{
    Q_OBJECT

    enum CaptureState
    {
        CS_Uncapture,
        CS_Capturing,
        CS_Edit
    };

public:
    explicit CaptureEditer(QWidget *parent = 0);

    ~CaptureEditer();

    void ShowCaptureEditer();

    QImage GetCaptureImage();

    QRect GetCaptureRect();

    void SetImage(const QImage& image);

    void DrawPreview(QPainter* painter);

protected:
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void sigExitCaptureEditer(bool succeed);

public slots:
    void slotReset();

    void slotFinishedCapture();

    void slotCancelCapture();

private:
    QImage background_image;

    CaptureState capture_state;

    AreaSelectedBox* area_selected_box;
    QMenu* menu;

    QPushButton* btn_ok;
    QPushButton* btn_cancel;

    QFont font;

    QRect selected_image_rect;
    QRect preview_rect;
    int preview_padding_size;

    QPoint mouse_origin_point;
    QPoint mouse_hover_point;

    bool is_start;
    bool is_button_clicked;
};

#endif // CAPTUREEDITER_H
