#ifndef FLATWIDGET_H
#define FLATWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QMouseEvent>
#include <QPushButton>

class FlatWidget : public QWidget
{
    Q_OBJECT

    enum MouseAction
    {
        MA_MoveWindow,
        MA_Resize,
        MA_Unknown
    };

public:
    explicit FlatWidget(QWidget *parent = 0);

    void LoadStyleSheet(const QString &path);

    void Init();

    void SetWindowTitle(const QString& title);

    void SetDeleteAfterClose(bool on);

    void SetLayout(QLayout* layout, int margin = 0);

    void resize(int w, int h);

    void resize(const QSize& size);

    void SetResizable(bool on) { is_resizable = on; }

    int GetPadding() const { return padding; }

    int GetHeadHeight() const { return head_height; }

    QWidget* GetCenterWidget(){ return center_widget; }

    void resizeEvent(QResizeEvent *event);

    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void sigCloseButtonClicked();

public slots:
    virtual void slotCloseButtonClicked();

protected:
    QPoint mouse_origin_point;
    QPoint window_origin_point;
    QSize  window_origin_size;

    QWidget* center_widget;
    QPushButton* btn_close;

    QString title;

    MouseAction mouse_action;

    int padding;
    int head_height;

    bool is_delete_after_close;
    bool is_resizable;
};

#endif // FLATWIDGET_H
