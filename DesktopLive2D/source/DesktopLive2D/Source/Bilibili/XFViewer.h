#ifndef XFVIEWER_H
#define XFVIEWER_H

//#define XFVIEWER_DEBUG

#include "XFSpider.h"

#include <QFont>
#include <QTimer>
#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>

#include <Source/Common.h>
#include <Source/Common/HideInSide.h>

struct XFViewerItem
{
    int Index;
    int TimeMin;
    bool Update;

    QString Time;
    QString Title;
};

class XFViewer : public QWidget
{
    Q_OBJECT
public:
    explicit XFViewer(QWidget *parent = 0);

    void SetXFDatas(const XFDataVector& datas);

    void UpdateViewer();

    int GetWeek(const QDateTime& dt);

    int GetHoverItemIndex(const QPoint& point);

    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *event);

    void mouseDoubleClickEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void leaveEvent(QEvent *);

signals:
    void sigSelectedXF();

    void sigUpdateXF(const QString& name);

#ifdef XFVIEWER_DEBUG
public slots:
    void slotTest();
#endif

private:
    void CreateItems(int week);

private:
    QPoint mouse_origin_point;
    QPoint window_origin_point;

#ifdef XFVIEWER_DEBUG
    QTimer timer;
    QDateTime cur_date_time;
#endif

    QMenu* menu;
    HideInSide* hide_in_side;

    QFont font;
    QString time;

    XFDataVector xf_datas;
    QVector<XFViewerItem> item_vector;

    bool is_no_xf;
    bool is_move_window;

    int current_weel;
    int current_index;
    int next_index;
    int item_height;
    int progress;
    int padding;
    int offset;
};

#endif // XFVIEWER_H
