#ifndef POPUPINFOMATIONVIEW_H
#define POPUPINFOMATIONVIEW_H

#include <QTimer>
#include <QQueue>
#include <QWidget>
#include <QPainter>
#include <QTimeLine>
#include <QMouseEvent>


class InfomationItem : public QObject
{
    Q_OBJECT
public:
    InfomationItem(int dpi, int maxWidth, int exist_time_ms, QObject *parent = 0);

    ~InfomationItem();

    void Draw(QPainter &painter);

    void SetText(const QString &text);

    void SetArrows(bool exist);

    QSize GetSize();

    void MoveTo(int x, int y);

    void ActionMoveTo(int x, int y);

private:
    void DrawBorder(QPainter &painter);

    void DrawText(QPainter &painter);

signals:
    void sigTimeOut();

public slots:
    void slotFrameChanged(int percent);

private:
    QTimer* timer;
    QString text;

    QFont font;

    int font_weight;
    int font_width;
    int max_width;
    int border_size;

    bool is_exist_arrows;

    QSize size;
    QSize text_size;

    QPoint begin_point;
    QPoint end_point;
    QPoint current_point;
};

//--------------------------------------------------------------
// PopupInfomationView
//--------------------------------------------------------------

class PopupInfomationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PopupInfomationWidget(QWidget *parent = 0);

    void SetLinkWidget(QWidget* widget);

    void AppendText(const QString &text, int exist_time_ms);

    void paintEvent(QPaintEvent *);

    bool eventFilter(QObject *, QEvent *event);

public:
    void Clear();

    void Clip(int count);

    void ShowAt(const QPoint& pos);

    void ShowAt(int x, int y);

public slots:
    void slotStep(int num);

    void slotRemoveInfomationItem();

private:
    QTimeLine timeline;
    QQueue<InfomationItem*> infomation_queue;

    int dpi;
    int border_size;
};

#endif // POPUPINFOMATIONVIEW_H
