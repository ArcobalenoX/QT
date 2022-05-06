#include "XFManager.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>

#include <Source/Common/PathHelper.h>

XFManager::XFManager(QObject *parent)
    : QObject(parent)
    , xf_spider(nullptr)
    , xf_viewer(nullptr)
    , xf_selected_dialog(nullptr)
{
    /* 定时器，每分钟刷新，驱动任务进行 */
    connect(&timer, SIGNAL(timeout()), this, SLOT(slotUpdate()));
    timer.setInterval(1000 * 60);
    timer.start();

    xf_viewer = new XFViewer;
    xf_viewer->move(Global::UserData.XFViewPoint);
    xf_viewer->show();

    connect(xf_viewer, SIGNAL(sigSelectedXF()), this, SLOT(slotShowXFSelectedDialog()));
    connect(xf_viewer, SIGNAL(sigUpdateXF(QString)), this, SIGNAL(sigUpdateXF(QString)));

    this->Init();

    /* 创建爬虫 */
    xf_spider = new XFSpider(this);
    xf_spider->Start();

    connect(xf_spider, SIGNAL(sigFinished()), this, SLOT(slotSpiderFinished()));
}

XFManager::~XFManager()
{
    delete xf_viewer;
    if( xf_selected_dialog ) delete xf_selected_dialog;

    Debug::Printf("Delete XFManager");
}

void XFManager::Init()
{
    xf_datas = XFSpider::LoadXFDataWithVector();

    xf_favorite_names = this->LoadFavoriteXFNames();
    xf_favorite_datas = this->GetFavoriteXFDatas(xf_datas, xf_favorite_names, xf_selected_table);

    xf_viewer->SetXFDatas(xf_favorite_datas);
    xf_viewer->UpdateViewer();
}

QStringList XFManager::LoadFavoriteXFNames()
{
    QStringList list;

    QFile file(PathHelper::GetXFDataDirPath() + "favorite.json");
    if( file.open(QFile::ReadOnly) )
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

        QJsonArray array = doc.array();
        for(int i = 0; i < array.size(); i++)
        {
            list << array.at(i).toString();
        }
        file.close();
    }
    return list;
}

bool XFManager::SaveFavoriteXFNames(const QStringList& list)
{
    QFile file(PathHelper::GetXFDataDirPath() + "favorite.json");

    if( file.open(QFile::WriteOnly) )
    {
        QJsonArray array;
        QJsonDocument doc;

        for(int i = 0; i < list.size(); i++)
        {
            array.append(QJsonValue(list[i]));
        }
        doc.setArray(array);
        file.write(doc.toJson());

        file.close();
        return true;
    }
    return false;
}

void XFManager::UpdateFavoriteXFNames(const XFDataVector& list, QStringList& names)
{
    for(QStringList::iterator it = names.begin(); it != names.end(); /* ++it */)
    {
        bool find = false;

        for(int j = 0; j < list.size(); j++)
        {
            if( list[j].Title.compare((*it)) == 0 )
            {
                find = true; break;
            }
        }
        /* 删除失效的元素 */
        if( find == false )
        {
            it = names.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

XFDataVector XFManager::GetFavoriteXFDatas(const XFDataVector& list, const QStringList& names, QVector<bool> &selected_table)
{
    XFDataVector favorite_xf_datas;

    /* 初始化选择表 */
    selected_table.resize(list.size());

    for(int i = 0; i < selected_table.size(); i++)
    {
        selected_table[i] = false;
    }

    /* 获取追番数据、更新选择表 */
    for(int i = 0; i < names.size(); i++)
    {
        for(int j = 0; j < list.size(); j++)
        {
            if( list[j].Title.compare(names[i]) == 0 )
            {
                favorite_xf_datas << list[j];
                selected_table[j] = true;
            }
        }
    }
    return favorite_xf_datas;
}

void XFManager::slotSpiderFinished()
{
    delete xf_spider;
    xf_spider = nullptr;

    /* 爬虫获取新数据结束后，更新数据 */
    xf_datas = XFSpider::LoadXFDataWithVector();

    this->UpdateFavoriteXFNames(xf_datas, xf_favorite_names);
    this->SaveFavoriteXFNames(xf_favorite_names);

    xf_favorite_datas = this->GetFavoriteXFDatas(xf_datas, xf_favorite_names, xf_selected_table);

    xf_viewer->SetXFDatas(xf_favorite_datas);
    xf_viewer->UpdateViewer();
}

void XFManager::slotShowXFSelectedDialog()
{
    if( xf_selected_dialog ) return;

    xf_selected_dialog = new XFSelectedDialog;
    connect(xf_selected_dialog, SIGNAL(sigCloseButtonClicked()), this, SLOT(slotDeleteXFSelectedDialog()));

    xf_selected_dialog->SetDeleteAfterClose(true);
    xf_selected_dialog->SetXFDatas(xf_datas, xf_selected_table);
    xf_selected_dialog->show();
}

void XFManager::slotDeleteXFSelectedDialog()
{
    QVector<int> indices = xf_selected_dialog->GetFavoriteXFIndices();
    xf_selected_dialog = nullptr;

    xf_favorite_names.clear();
    for(int i = 0; i < indices.size(); i++)
    {
        xf_favorite_names << xf_datas[indices[i]].Title;
    }

    /* 获取新的追番数据 */
    xf_favorite_datas = this->GetFavoriteXFDatas(xf_datas, xf_favorite_names, xf_selected_table);
    this->SaveFavoriteXFNames(xf_favorite_names);

    /* 使用新的数据更新视图 */
    xf_viewer->SetXFDatas(xf_favorite_datas);
    xf_viewer->UpdateViewer();
}

void XFManager::slotUpdate()
{
    xf_viewer->UpdateViewer();
}
