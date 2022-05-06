#ifndef SPIDER_H
#define SPIDER_H

#include <QMap>
#include <QList>
#include <QTimer>
#include <QQueue>
#include <QObject>
#include <QVector>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <QtWebEngineWidgets/QWebEnginePage>
#include <QtWebEngineWidgets/QWebEngineView>

#include <Source/Common/Debug.h>

class ImageDownloadManager;

struct PreviewData
{
    QString ImageUrl;
    QString CV;
    QString UpdateInfo;
    QString DescriptionInfo;
};

struct XFData
{
    int Index;
    QString Week;
    QString DateTime;
    QString Title;
    QString ImageUrl;
    QString CoverUrl;
    QString UpdateNum;

    PreviewData ExtendData;
};

typedef QVector<XFData> XFDataVector;
typedef QMap<QString, XFData> XFDataMap;


//---------------------------------------------------------------------
// Spider
// Spider 是一个爬取 B 站新番数据的对爬虫对象，调用 Start 方法开始，结束时发出
// sigFinished 信号表示爬虫已经完成数据获取的工作。
//---------------------------------------------------------------------
class XFSpider : public QObject
{
    Q_OBJECT

    struct TaskData
    {
        QString Name;
        QString Url;
    };

public:
    static bool TimelineDataLessThan(const XFData& d1, const XFData& d2)
    {
        return d1.Index < d2.Index;
    }

public:
    explicit XFSpider(QObject *parent = 0);

    virtual ~XFSpider();

    void Start();

    static XFDataMap LoadXFData();

    static XFDataVector LoadXFDataWithVector();

    static XFDataVector XFDataMapTOXFDataVector();

    static XFDataMap ParseXFDataHtml(const QString& html);

    static PreviewData ParsePreviewHtml(const QString& html);

private:
    QJsonObject XFDataToJsonObject(const XFData& data);

    QJsonObject PreviewDataToJsonObject(const PreviewData& data);

    bool SaveXFDatas(const XFDataMap datas);

    bool UpdateXFDatas(const QString& html);

    void DownloadError(QNetworkReply* reply);

    void StartDownloadQueue(const QQueue<TaskData>& queue, bool download_image = true);

    void DownloadQueueStep();

    static int WeekToInt(const QString& week);

    static int DateTimeToInt(const QString& time);

signals:
    void sigFinished();

public slots:
    void slotReloadWebpage();

    void slotLoadWebpageFinished(bool ok);

    void slotNetworkFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager manager;
    QWebEnginePage* web_engine_page;

    QTimer* timer;
    XFDataMap xf_datas;

    int cover_download_index;

    bool is_download_images;

    QString current_task_name;
    QQueue<TaskData> active_download_queue;
    QQueue<TaskData> image_download_queue;
    QQueue<TaskData> cover_download_queue;
};

#endif // SPIDER_H
