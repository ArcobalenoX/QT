#ifndef MODELSELECTEDWIDGET_H
#define MODELSELECTEDWIDGET_H

#include <QDir>
#include <QEvent>
#include <QLabel>
#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QListView>
#include <QDropEvent>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QStandardItem>
#include <QDragEnterEvent>
#include <QStandardItemModel>
#include <QFileDialog>

#include <Source/Common/FlatWidget.h>


//--------------------------------------------------------------------
// ModelSelectedWidget
//--------------------------------------------------------------------
class FModelSelectedWidget : public FlatWidget
{
    Q_OBJECT
public:
    explicit FModelSelectedWidget(QWidget *parent = 0);

    ~FModelSelectedWidget();

    void AppendData(const QString& name, QPixmap* pixmap);

    void ClearData();

    void dragEnterEvent(QDragEnterEvent *event);

    void dropEvent(QDropEvent *event);

private:
    void AddNewModels(const QStringList& path_list);

    bool IsAcceptDir(const QDir& dir);

    bool CopyDir(const QString& source_path, const QString& target_path);

public slots:
    void slotOpenModelSelectedDialog();

signals:
    void sigModelSelected(int index);

    void sigNewModels(const QStringList& path_list);

private:
    QListView* list_view;
    QStandardItemModel* standard_model;

    QLabel* image_view;
    QPushButton* btn_add_model;

    struct Item
    {
        QString name;
        QPixmap* pixmap;
    };
    QVector<Item> item_vector;
};

#endif // MODELSELECTEDWIDGET_H
