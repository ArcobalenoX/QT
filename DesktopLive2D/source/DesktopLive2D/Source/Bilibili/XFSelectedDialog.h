#ifndef XFSELECTEDDIALOG_H
#define XFSELECTEDDIALOG_H

#include "XFSpider.h"
#include <Source/Common/FlatWidget.h>

#include <QFont>
#include <QVector>
#include <QPainter>
#include <QTimeLine>
#include <QMouseEvent>

#include <QListView>
#include <QModelIndex>
#include <QItemDelegate>
#include <QStandardItemModel>

struct ItemData
{
    QImage* Image;
    QString Name;
    QString Info;

    QImage* PreviewImage;
    QString CV;
    QString UpdateInfo;
    QString Description;

    int Index;
};

class XFDelegate : public QItemDelegate
{
public:
    XFDelegate(QWidget* parent = 0);

    ~XFDelegate();

    void SetDatas(QVector<ItemData*>* datas);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;

private:
    QVector<ItemData*>* datas;

    QFont name_font;
    QFont info_font;
};

//----------------------------------------------------------------------
// XFSelectedDialogImpl
//----------------------------------------------------------------------
class XFSelectedDialogImpl : public QWidget
{
    Q_OBJECT
public:
    explicit XFSelectedDialogImpl(QWidget* parent = 0);

    ~XFSelectedDialogImpl();

    void Clear();

    void SetXFDatas(const XFDataVector& datas, const QVector<bool> &selected_table);

    QVector<int> GetFavoriteXFIndices();

    void MoveItem(QStandardItemModel* src_model, QStandardItemModel* dst_model, const QModelIndex& index);

    void paintEvent(QPaintEvent *);

    void mouseMoveEvent(QMouseEvent *event);

    void leaveEvent(QEvent *);

private:
    void StartShowAmination();

    void StartHideAnimation();

public slots:
    void slotLClicked(const QModelIndex& index);

    void slotRClicked(const QModelIndex& index);

    void slotItemActived(const QModelIndex& index);

    void slotAnimationUpdate(int value);

private:
    QListView* all_xflist_view;
    QListView* display_xflist_view;

    QStandardItemModel* all_model;
    QStandardItemModel* display_model;

    QVector<ItemData*> xf_datas;

    QRect animation_rect;
    QTimeLine animation;
    XFDelegate* delegate;

    QPoint mouse_hover_point;

    int active_index;
    bool is_enter;
    float animation_ratio;

    QFont font;
};


//----------------------------------------------------------------------
// XFSelectedDialog
//----------------------------------------------------------------------
class XFSelectedDialog : public FlatWidget
{
    Q_OBJECT
public:
    explicit XFSelectedDialog(QWidget *parent = 0);

    void SetXFDatas(const XFDataVector& datas, const QVector<bool>& selected_table);

    QVector<int> GetFavoriteXFIndices();

private:
    XFSelectedDialogImpl* impl;
};

#endif // XFSELECTEDDIALOG_H
