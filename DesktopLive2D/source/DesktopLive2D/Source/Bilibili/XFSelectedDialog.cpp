#include "XFSelectedDialog.h"
#include <Source/Common/Debug.h>
#include <Source/Common/PathHelper.h>

#include <QtAlgorithms>
#include <QVariant>

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

static const int ItemWidth = 225;
static const int ItemHeight = 50;
static const int ItemSpace  = 2;
static const int PreviewWidth = 225;
static const int PreviewSpace = 20;


XFDelegate::XFDelegate(QWidget* parent) : QItemDelegate(parent)
{
    name_font.setFamily(QString::fromLocal8Bit("Î¢ÈíÑÅºÚ"));
    name_font.setPointSize(10);

    info_font.setFamily(QString::fromLocal8Bit("Î¢ÈíÑÅºÚ"));
    info_font.setPointSize(8);
}

XFDelegate::~XFDelegate()
{
    Debug::Printf("Delete XFDelegate");
}

void XFDelegate::SetDatas(QVector<ItemData*>* datas)
{
    this->datas = datas;
}

void XFDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ItemData* data = datas->at(index.model()->data(index).toInt());

    bool hover = option.state & QStyle::State_MouseOver;
    QRect display_rect = option.rect;

    if( data->Image ){
        painter->drawImage(QRect(display_rect.topLeft(), QSize(ItemHeight, ItemHeight)), *(data->Image));
    }

    QRect rect = display_rect.adjusted(ItemHeight, 0, 0, 0);

    if( hover ){
        painter->fillRect(rect, QColor(255, 255, 255, 80));
        painter->fillRect(rect.adjusted(0, display_rect.height() - 2, 0, 0), QColor(255, 255, 255, 200));
    }
    else{
        painter->fillRect(rect, QColor(255, 255, 255, 16));
    }

    QRect text_rect = rect.adjusted(5, 5, -5, -5);

    painter->setFont(name_font); painter->setPen(Qt::white);
    painter->drawText(text_rect.adjusted(0, 0, 0, -text_rect.height() * 0.5f), Qt::AlignTop, data->Name);

    painter->setFont(info_font); painter->setPen(QColor(46, 204, 113, 255));
    painter->drawText(text_rect.adjusted(0,  text_rect.height() * 0.5f, 0, 0), Qt::AlignBottom, data->Info);
}

QSize XFDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ItemWidth, ItemHeight);
}

//----------------------------------------------------------------------
// XFSelectedDialogImpl
//----------------------------------------------------------------------
XFSelectedDialogImpl::XFSelectedDialogImpl(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);
    active_index = -1;
    animation_ratio = 0;
    is_enter = false;

    QString style_sheet = "";

    QFile file(PathHelper::GetStyleSheetPath() + "XFStyle.css");
    if( file.open(QFile::ReadOnly) ){
        style_sheet = file.readAll();
        file.close();
    }

    delegate = new XFDelegate(this);

    all_xflist_view = new QListView(this);
    all_xflist_view->setMouseTracking(true);
    all_xflist_view->setItemDelegate(delegate);
    all_xflist_view->setStyleSheet(style_sheet);
    all_xflist_view->setSpacing(ItemSpace * 0.5f);
    all_xflist_view->setEditTriggers(QListView::NoEditTriggers);
    all_xflist_view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    connect(all_xflist_view, SIGNAL(clicked(QModelIndex)), this, SLOT(slotLClicked(QModelIndex)));
    connect(all_xflist_view, SIGNAL(entered(QModelIndex)), this, SLOT(slotItemActived(QModelIndex)));

    display_xflist_view = new QListView(this);
    display_xflist_view->setMouseTracking(true);
    display_xflist_view->setItemDelegate(delegate);
    display_xflist_view->setStyleSheet(style_sheet);
    display_xflist_view->setSpacing(ItemSpace * 0.5f);
    display_xflist_view->setEditTriggers(QListView::NoEditTriggers);
    display_xflist_view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    connect(display_xflist_view, SIGNAL(clicked(QModelIndex)), this, SLOT(slotRClicked(QModelIndex)));
    connect(display_xflist_view, SIGNAL(entered(QModelIndex)), this, SLOT(slotItemActived(QModelIndex)));

    all_model = new QStandardItemModel(this);
    all_model->setColumnCount(1);

    display_model = new QStandardItemModel(this);
    display_model->setColumnCount(1);

    all_xflist_view->setModel(all_model);
    display_xflist_view->setModel(display_model);

    QVBoxLayout* vlayout1 = new QVBoxLayout;
    QVBoxLayout* vlayout2 = new QVBoxLayout;

    QLabel* label1 = new QLabel(QString::fromLocal8Bit("ËùÓÐÐÂ·¬"), this);
    QLabel* label2 = new QLabel(QString::fromLocal8Bit("ÒÑ×·ÐÂ·¬"), this);

    label1->setStyleSheet(QString::fromLocal8Bit("QLabel{ font-size: 14px; font-family: Î¢ÈíÑÅºÚ }"));
    label2->setStyleSheet(QString::fromLocal8Bit("QLabel{ font-size: 14px; font-family: Î¢ÈíÑÅºÚ }"));

    label1->setFixedHeight(20);
    label2->setFixedHeight(20);

    vlayout1->addWidget(label1);
    vlayout1->addWidget(all_xflist_view);

    vlayout2->addWidget(label2);
    vlayout2->addWidget(display_xflist_view);

    QHBoxLayout* hlayout = new QHBoxLayout(this);
    hlayout->addLayout(vlayout1);
    hlayout->addSpacing(PreviewWidth + PreviewSpace * 2);
    hlayout->addLayout(vlayout2);

    this->setLayout(hlayout);

    font.setFamily(QString::fromLocal8Bit("Î¢ÈíÑÅºÚ"));
    font.setPointSize(8);

    animation.setDuration(300);
    animation.setUpdateInterval(1000 / 60.0f);
    animation.setEasingCurve(QEasingCurve::OutSine);
    connect(&animation, SIGNAL(frameChanged(int)), this, SLOT(slotAnimationUpdate(int)));
}

XFSelectedDialogImpl::~XFSelectedDialogImpl()
{
    this->Clear();

    all_model->clear();
    display_model->clear();

    Debug::Printf("Delete XFSelectedDialogImpl");
}

void XFSelectedDialogImpl::Clear()
{
    for(int i = 0; i < xf_datas.size(); i++)
    {
        delete xf_datas[i]->Image;
        delete xf_datas[i]->PreviewImage;
        delete xf_datas[i];
    }
    xf_datas.clear();
}

void XFSelectedDialogImpl::SetXFDatas(const XFDataVector& datas, const QVector<bool> &selected_table)
{
    this->Clear();

    int selected_count = 0;
    for(int i = 0; i < selected_table.size(); i++)
    {
        if( selected_table[i] ) selected_count++;
    }

    all_model->setRowCount(datas.size() - selected_count);
    display_model->setRowCount(selected_count);

    int i1 = 0, i2 = 0;

    for(int i = 0; i < datas.size(); i++)
    {
        ItemData* item_data = new ItemData;

        QString path = PathHelper::GetXFDataDirPath() + datas[i].Title + ".png";
        if( QFile::exists(path) )
        {
            item_data->Image = new QImage;
            item_data->Image->load(path);
        }
        else
        {
            item_data->Image = nullptr;
        }

        item_data->Index = datas[i].Index;
        item_data->Name  = datas[i].Title;
        item_data->Info  = QString("%1  %2").arg(datas[i].Week, datas[i].DateTime);

        path = PathHelper::GetXFDataDirPath() + datas[i].Title + QString::fromLocal8Bit("_·âÃæ.png");
        if( QFile::exists(path) )
        {
            item_data->PreviewImage = new QImage;
            item_data->PreviewImage->load(path);
        }
        else
        {
            item_data->PreviewImage = nullptr;
        }
        item_data->CV          = datas[i].ExtendData.CV;
        item_data->UpdateInfo  = datas[i].ExtendData.UpdateInfo;
        item_data->Description = datas[i].ExtendData.DescriptionInfo;

        xf_datas.push_back(item_data);

        if( selected_table[i] )
        {
            QModelIndex index = display_model->index(i2++, 0, QModelIndex());
            display_model->setData(index, i);
        }
        else
        {
            QModelIndex index = all_model->index(i1++, 0, QModelIndex());
            all_model->setData(index, i);
        }
    }

    delegate->SetDatas(&xf_datas);

    all_xflist_view->update();
    display_xflist_view->update();
}

QVector<int> XFSelectedDialogImpl::GetFavoriteXFIndices()
{
    QVector<int> list;
    for(int i = 0; i < display_model->rowCount(); i++)
    {
        list << display_model->index(i, 0).data().toInt();
    }
    return list;
}

void XFSelectedDialogImpl::StartShowAmination()
{
    if( is_enter || active_index == -1 ) return;

    is_enter = true;
    animation.stop();
    animation.setStartFrame(animation_ratio * 100);
    animation.setEndFrame(100);
    animation.start();
}

void XFSelectedDialogImpl::StartHideAnimation()
{
    if( is_enter == false || active_index == -1 ) return;

    is_enter = false;
    animation.stop();
    animation.setStartFrame(animation_ratio * 100);
    animation.setEndFrame(0);
    animation.start();
}

void XFSelectedDialogImpl::MoveItem(QStandardItemModel* src_model, QStandardItemModel* dst_model, const QModelIndex& index)
{
    QVector<bool> actives(xf_datas.size(), false);

    for(int i = 0; i < src_model->rowCount(); i++)
    {
        if( src_model->index(i, 0).row() != index.row() )
        {
            actives[src_model->index(i, 0).data().toInt()] = true;
        }
    }

    src_model->setRowCount(src_model->rowCount() - 1);
    dst_model->setRowCount(dst_model->rowCount() + 1);

    int i1 = 0, i2 = 0;

    for(int i = 0; i < actives.size(); i++)
    {
        if( actives[i] )
        {
            src_model->setData(src_model->index(i1++, 0), i);
        }
        else
        {
            dst_model->setData(dst_model->index(i2++, 0), i);
        }
    }
}

void XFSelectedDialogImpl::slotLClicked(const QModelIndex &index)
{
    this->MoveItem(all_model, display_model, index);
}

void XFSelectedDialogImpl::slotRClicked(const QModelIndex &index)
{
    this->MoveItem(display_model, all_model, index);
}

void XFSelectedDialogImpl::slotItemActived(const QModelIndex& index)
{
    active_index = index.data().toInt();
    this->update();
}

void XFSelectedDialogImpl::slotAnimationUpdate(int value)
{
    animation_ratio = value / 100.0f;
    font.setPointSize(8 + animation_ratio * 2);
    this->update();
}

void XFSelectedDialogImpl::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    animation_rect.setRect((this->width() - PreviewWidth) * 0.5f, this->y() + 300, PreviewWidth, this->height() - 300);

    int x = (this->width() - PreviewWidth) * 0.5f;
    int y = this->pos().y() - 300 * animation_ratio;
    int w = PreviewWidth;
    int h = this->height();

    painter.drawLine(x - 0 - PreviewSpace * 0.5f, 0, x - 0 - PreviewSpace * 0.5f, this->height());
    painter.drawLine(x + w + PreviewSpace * 0.5f, 0, x + w + PreviewSpace * 0.5f, this->height());

    if( active_index != -1 )
    {
        ItemData* item_data = xf_datas[active_index];

        if( xf_datas[active_index]->PreviewImage )
        {
            painter.setOpacity(1 - animation_ratio);
            painter.drawImage(QRect(x, y, w, 300), *(item_data->PreviewImage));
        }

        painter.setFont(font);
        QRect text_rect(x, y + 300, w, h);
        text_rect = text_rect.adjusted(5, 5, -5, -5);

        QString text = item_data->UpdateInfo + "\n\n" +
                QString::fromLocal8Bit("CV£º") + item_data->CV + "\n\n" +
                QString::fromLocal8Bit("¼ò½é£º") + item_data->Description;

        painter.setOpacity(1);
        painter.drawText(text_rect, text);
    }
}

void XFSelectedDialogImpl::mouseMoveEvent(QMouseEvent *event)
{
    mouse_hover_point = event->pos();
    this->update();

    if( animation_rect.contains(mouse_hover_point) )
    {
        this->StartShowAmination();
    }
    else
    {
        this->StartHideAnimation();
    }
}

void XFSelectedDialogImpl::leaveEvent(QEvent *)
{
    this->StartHideAnimation();
}

//----------------------------------------------------------------------
// XFSelectedDialog
//----------------------------------------------------------------------
XFSelectedDialog::XFSelectedDialog(QWidget *parent) : FlatWidget(parent)
{
    this->SetWindowTitle(QString::fromLocal8Bit("×··¬Ñ¡Ôñ"));
    this->resize((ItemWidth + 30) * 2 + PreviewWidth + PreviewSpace * 2, 600);

    QVBoxLayout* vlayout = new QVBoxLayout(this);

    impl = new XFSelectedDialogImpl;
    vlayout->addWidget(impl);

    this->SetLayout(vlayout);
}

void XFSelectedDialog::SetXFDatas(const XFDataVector &datas, const QVector<bool> &selected_table)
{
    impl->SetXFDatas(datas, selected_table);
}

QVector<int> XFSelectedDialog::GetFavoriteXFIndices()
{
    return impl->GetFavoriteXFIndices();
}
