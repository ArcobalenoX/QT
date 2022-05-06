#ifndef XFMANAGER_H
#define XFMANAGER_H

#include <QTimer>
#include <QObject>

#include "XFSpider.h"
#include "XFViewer.h"
#include "XFSelectedDialog.h"

class XFManager : public QObject
{
    Q_OBJECT

    struct TaskData
    {
        int Index;
        int TimeMin;
    };

public:
    explicit XFManager(QObject *parent = 0);

    ~XFManager();

    void Init();

private:
    QStringList LoadFavoriteXFNames();

    bool SaveFavoriteXFNames(const QStringList& list);

    void UpdateFavoriteXFNames(const XFDataVector& list, QStringList &names);

    XFDataVector GetFavoriteXFDatas(const XFDataVector& list, const QStringList& names, QVector<bool>& selected_table);

signals:
    void sigUpdateXF(const QString& name);

public slots:
    void slotUpdate();

    void slotSpiderFinished();

    void slotShowXFSelectedDialog();

    void slotDeleteXFSelectedDialog();

private:
    XFSpider* xf_spider;
    XFViewer* xf_viewer;

    XFSelectedDialog* xf_selected_dialog;

    QTimer timer;
    XFDataVector xf_datas;
    XFDataVector xf_favorite_datas;
    QStringList  xf_favorite_names;

    QVector<bool> xf_selected_table;
};

#endif // XFMANAGER_H
