#include "XFSpider.h"

#include <QFile>
#include <QRegExp>
#include <QtAlgorithms>


#include <Source/Common/PathHelper.h>

XFSpider::XFSpider(QObject *parent) : QObject(parent)
{
    timer = nullptr;

    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotNetworkFinished(QNetworkReply*)));

    web_engine_page = new QWebEnginePage(this);
    connect(web_engine_page, SIGNAL(loadFinished(bool)), this, SLOT(slotLoadWebpageFinished(bool)));
}

XFSpider::~XFSpider()
{
    qDebug("delete XFSpider");
}

void XFSpider::Start()
{
    QString url = "https://bangumi.bilibili.com/anime/timeline";
    web_engine_page->load(QUrl(url));
}

void XFSpider::StartDownloadQueue(const QQueue<TaskData>& queue, bool download_image)
{
    is_download_images = download_image;
    active_download_queue = queue;
    this->DownloadQueueStep();
}

void XFSpider::DownloadQueueStep()
{
    if( active_download_queue.isEmpty() ) return;

    TaskData data = active_download_queue.dequeue();
    current_task_name = data.Name;

    if( Debug::DownloadDebugInfo ) Debug::Printf(QString("Start download: %1").arg(data.Url));
    manager.get(QNetworkRequest(QUrl(data.Url)));
}

XFDataMap XFSpider::LoadXFData()
{
    QFile file(PathHelper::GetXFDataDirPath() + "data.json");
    if( file.open(QFile::ReadOnly) == false ){
        return XFDataMap();
    }

    /* 从本地加载新番数据的 Json 文件 */
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if( doc.isArray() == false ) return XFDataMap();

    XFDataMap xfdatas;

    /* 读取 Json 中的数据 */
    QJsonArray array = doc.array();
    for(int i = 0; i < array.size(); i++){
        QJsonObject obj = array[i].toObject();

        XFData data;
        data.Index      = obj["Index"].toInt();
        data.Title      = obj["Title"].toString();
        data.Week       = obj["Week"].toString();
        data.DateTime   = obj["DateTime"].toString();
        data.CoverUrl   = obj["CoverUrl"].toString();
        data.ImageUrl   = obj["ImageUrl"].toString();
        data.UpdateNum  = obj["UpdateNum"].toString();

        PreviewData preview_data;
        QJsonObject preview_obj      = obj["PreviewData"].toObject();
        preview_data.ImageUrl        = preview_obj["PreviewUrl"].toString();
        preview_data.CV              = preview_obj["CV"].toString();
        preview_data.UpdateInfo      = preview_obj["UpdateInfo"].toString();
        preview_data.DescriptionInfo = preview_obj["DescriptionInfo"].toString();

        data.ExtendData = preview_data;
        xfdatas[data.Title] = data;
    }
    return xfdatas;
}

XFDataVector XFSpider::LoadXFDataWithVector()
{
    XFDataMap map_data = LoadXFData();

    /* 排序 */
    QMap<int, XFData> sort_datas;
    for(XFDataMap::iterator it = map_data.begin(); it != map_data.end(); ++it){
        sort_datas[it.value().Index] = it.value();
    }

    XFDataVector vector_data;
    for(QMap<int, XFData>::iterator it = sort_datas.begin(); it != sort_datas.end(); ++it){
        vector_data << it.value();
    }
    return vector_data;
}

QJsonObject XFSpider::XFDataToJsonObject(const XFData &data)
{
    QJsonObject obj;
    obj["Index"]     = data.Index;
    obj["Title"]     = data.Title;
    obj["Week"]      = data.Week;
    obj["DateTime"]  = data.DateTime;
    obj["CoverUrl"]  = data.CoverUrl;
    obj["ImageUrl"]  = data.ImageUrl;
    obj["UpdateNum"] = data.UpdateNum;

    return obj;
}

QJsonObject XFSpider::PreviewDataToJsonObject(const PreviewData &data)
{
    QJsonObject obj;
    obj["PreviewUrl"]      = data.ImageUrl;
    obj["CV"]              = data.CV;
    obj["UpdateInfo"]      = data.UpdateInfo;
    obj["DescriptionInfo"] = data.DescriptionInfo;

    return obj;
}

bool XFSpider::SaveXFDatas(const XFDataMap datas)
{
    QMap<int, XFData> sort_datas;
    for(XFDataMap::const_iterator it = datas.begin(); it != datas.end(); ++it){
        sort_datas[it.value().Index] = it.value();
    }

    QJsonDocument doc;
    QJsonArray array;

    for(QMap<int, XFData>::iterator it = sort_datas.begin(); it != sort_datas.end(); ++it){
        QJsonObject main_obj   = this->XFDataToJsonObject(it.value());
        QJsonObject extend_obj = this->PreviewDataToJsonObject(it.value().ExtendData);

        main_obj["PreviewData"] = extend_obj;
        array.append(main_obj);
    }
    doc.setArray(array);

    QFile file(PathHelper::GetXFDataDirPath() + "data.json");
    if( file.open(QFile::WriteOnly) ){
        file.write(doc.toJson());
        file.close();
        return true;
    }
    return false;
}

XFDataMap XFSpider::ParseXFDataHtml(const QString& html)
{
    XFDataMap xfdatas;;

    QRegExp begin("<div class=\"timeline-container\">");
    int pos = begin.indexIn(html);
    int begin_pos = pos;

    QRegExp end("<div class=\"footer\">");
    int end_pos = end.indexIn(html, pos);

    QRegExp week("<span class=\"t-week\">(.{2})</span>");
    week.setMinimal(true);

    QRegExp separator("<div class=\"day-wrap");
    separator.setMinimal(true);

    QRegExp date_time("<div class=\"date-time\">(.+)</div>");
    date_time.setMinimal(true);

    QRegExp cover_url("<a href=\"(.+)\"");
    cover_url.setMinimal(true);

    QRegExp title("title=\"(.{,50})\"");
    title.setMinimal(true);

    QRegExp image_url("<img src=\"(.+)\"");
    image_url.setMinimal(true);

    QRegExp update_num("<div class=\"b-label( published)?\">(.+)</div>");
    update_num.setMinimal(true);

    /* 获取星期（页面共有三个星期的内容） */
    int idx = 0;
    QString weeks[3];
    while( (pos = week.indexIn(html, pos)) != -1 && pos < end_pos )
    {
        pos += week.matchedLength();
        weeks[idx++] = week.cap(1);
    }

    /* 获取星期对应内容的分割点 */
    idx = 0;
    pos = begin_pos;
    int separator_pos[3];

    while( (pos = separator.indexIn(html, pos)) != -1 )
    {
        separator_pos[idx++] = pos;
        pos += separator.matchedLength();
    }

    /* 获取内容 */
    idx = 0;
    pos = separator_pos[idx++];
    while( (pos = date_time.indexIn(html, pos)) != -1 && pos < end_pos )
    {
        if( idx < 3 && pos > separator_pos[idx] )
        {
            pos = separator_pos[idx];
            idx ++;
            continue;
        }
        XFData data;
        data.Week = weeks[idx - 1];

        /* 新番更新时间 */
        pos += date_time.matchedLength();
        data.DateTime = date_time.cap(1);

        /* 新番封面链接 */
        pos  = cover_url.indexIn(html, pos);
        pos += cover_url.matchedLength();
        data.CoverUrl = "https://bangumi.bilibili.com" + cover_url.cap(1);

        /* 新番名称 */
        pos  = title.indexIn(html, pos);
        pos += title.matchedLength();
        data.Title = title.cap(1);

        /* 新番封面链接 */
        pos  = image_url.indexIn(html, pos);
        pos += image_url.matchedLength();
        data.ImageUrl = image_url.cap(1);

        /* 新番更新集数 */
        pos  = update_num.indexIn(html, pos);
        pos += update_num.matchedLength();
        data.UpdateNum = update_num.cap(update_num.captureCount());

        /* 插入到新番表 */
        data.Index = WeekToInt(data.Week) * 10000 + DateTimeToInt(data.DateTime);
        xfdatas[data.Title] = data;
    }
    return xfdatas;
}

PreviewData XFSpider::ParsePreviewHtml(const QString& html)
{
    PreviewData data;

    QRegExp begin("<div class=\"info-content\">");
    begin.setMinimal(true);

    QRegExp preview("<img src=\"(.+)\"");
    preview.setMinimal(true);

    QRegExp info_update_begin("<div class=\"info-row info-update\">");
    QRegExp info_update("<span>(.+)</span>");
    info_update.setMinimal(true);

    QRegExp cv_begin("<div class=\"info-row info-cv\">");
    cv_begin.setMinimal(true);

    QRegExp cv_end("</em>");
    cv_end.setMinimal(true);

    QRegExp cv("</span>(.+)</span>");
    cv.setMinimal(true);

    QRegExp desc("<div class=\"info-desc\">(.+)</div>");
    desc.setMinimal(true);

    int pos = 0, origin = 0;
    pos  = begin.indexIn(html, pos);
    pos += begin.matchedLength();

    /* 获取预览区链接 */
    pos  = preview.indexIn(html, pos);
    pos += preview.matchedLength();
    data.ImageUrl = "https:" + preview.cap(1);
    //data.ImageUrl = data.ImageUrl.left(data.ImageUrl.lastIndexOf("_"));

    /* 读取更新信息 */
    pos  = info_update_begin.indexIn(html, pos);
    pos += info_update_begin.matchedLength();

    pos  = info_update.indexIn(html, pos);
    pos += info_update.matchedLength();
    data.UpdateInfo = info_update.cap(1);
    data.UpdateInfo = data.UpdateInfo.replace(" ", "");
    data.UpdateInfo = data.UpdateInfo.replace("\n", "");

    pos  = info_update.indexIn(html, pos);
    pos += info_update.matchedLength();
    data.UpdateInfo = data.UpdateInfo + "  " + info_update.cap(1);

    /* 读取 CV */
    pos  = cv_begin.indexIn(html, pos);
    pos += cv_begin.matchedLength();
    origin = pos;

    int end_pos  = cv_end.indexIn(html, pos);
    end_pos += cv_end.matchedLength();

    while( (pos = cv.indexIn(html, pos)) != -1 && pos < end_pos ){
        pos += cv.matchedLength();
        data.CV = data.CV + cv.cap(1) + " ";
    }

    /* 获取简介 */
    pos  = desc.indexIn(html, origin);
    pos += desc.matchedLength();
    data.DescriptionInfo = desc.cap(1);

    return data;
}

bool XFSpider::UpdateXFDatas(const QString& html)
{
    XFDataMap old_datas;

    XFDataMap local_datas = this->LoadXFData();
    XFDataMap parse_datas = this->ParseXFDataHtml(html);

    int active_week7[7] = { 0 };
    int active_week3[3] = { 0 };

    /* 判断解析的数据属于是星期 X 的内容 */
    for(XFDataMap::iterator it = parse_datas.begin(); it != parse_datas.end(); ++it)
    {
        xf_datas[it.key()] = it.value();

        active_week7[WeekToInt(it.value().Week) - 1] = 1;
    }

    int idx = 0;
    for(int i = 0; i < 7; i++)
    {
        if( active_week7[i] ) active_week3[idx++] = i + 1;
    }

    /* 从本地的新番文件中获取相应星期的数据 */
    for(XFDataMap::iterator it = local_datas.begin(); it != local_datas.end(); ++it)
    {
        int index = it.value().Index;

        bool find = false;
        find |= (index >= active_week3[0] * 10000 && index < (active_week3[0] + 1) * 10000);
        find |= (index >= active_week3[1] * 10000 && index < (active_week3[1] + 1) * 10000);
        find |= (index >= active_week3[2] * 10000 && index < (active_week3[2] + 1) * 10000);

        if( find )
        {
            old_datas[it.key()] = it.value();
        }
        else
        {
            xf_datas[it.key()] = it.value();
        }
    }

    /* 删除过期的封面 */
    for(XFDataMap::iterator it = old_datas.begin(); it != old_datas.end(); ++it)
    {
        if( parse_datas.find(it.key()) == parse_datas.end() )
        {
            QString path = PathHelper::GetXFDataDirPath() + it.key() + ".png";
            if( QFile::exists(path) ) QFile::remove(path);

            path = PathHelper::GetXFDataDirPath() + it.key() + QString::fromLocal8Bit("_封面.png");
            if( QFile::exists(path) ) QFile::remove(path);
        }
        else
        {
            xf_datas[it.key()].ExtendData = it.value().ExtendData;
        }
    }

    /* 为新添加的数据下载详细信息 */
    for(XFDataMap::iterator it = parse_datas.begin(); it != parse_datas.end(); ++it)
    {
        if( old_datas.find(it.key()) == old_datas.end() )
        {
            TaskData d1;
            d1.Name = it.key();
            d1.Url  = it.value().ImageUrl;
            image_download_queue.enqueue(d1);

            TaskData d2;
            d2.Name = it.key();
            d2.Url  = it.value().CoverUrl;
            cover_download_queue.enqueue(d2);
        }
    }
    if( cover_download_queue.size() > 0 )
    {
        this->StartDownloadQueue(cover_download_queue, false);
        return true;
    }
    return false;
}

void XFSpider::DownloadError(QNetworkReply* reply)
{
    Debug::Printf(reply->errorString());

    QNetworkReply::NetworkError error = reply->error();
    switch (error) {
    case QNetworkReply::HostNotFoundError:
        Debug::Printf("NetworkReply Error: HostNotFoundError");     // 检查网络连接
        break;
    case QNetworkReply::TimeoutError:
        Debug::Printf("NetworkReply Error: TimeoutError");          // 连接超时错误
        break;
    case QNetworkReply::ContentNotFoundError:
        Debug::Printf("NetworkReply Error: ContentNotFoundError");  // 不存在的内容错误
        break;
    case QNetworkReply::ProtocolUnknownError:
        Debug::Printf("NetworkReply Error: ProtocolUnknownError");  // 未知协议错误
        break;
    case QNetworkReply::UnknownContentError:
        Debug::Printf("NetworkReply Error: UnknownContentError");   // 未知内容错误
        break;
    }

    QUrl faild_url = reply->request().url();
    int http_status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    Debug::Printf(QString("Network error: %1, faild download %2 \n").arg(http_status).arg(faild_url.toString()));
}

int XFSpider::WeekToInt(const QString& week)
{
    static const int week_num = 7;
    static QString weeks[week_num] = {
        QString::fromLocal8Bit("周一"),
        QString::fromLocal8Bit("周二"),
        QString::fromLocal8Bit("周三"),
        QString::fromLocal8Bit("周四"),
        QString::fromLocal8Bit("周五"),
        QString::fromLocal8Bit("周六"),
        QString::fromLocal8Bit("周日")
    };

    for(int i = 0; i < week_num; i++)
    {
        if( weeks[i].compare(week) == 0 )
        {
            return i + 1;
        }
    }
    return -1;
}

int XFSpider::DateTimeToInt(const QString& time)
{
    QStringList list = time.split(':');
    int h = list[0].toInt();
    int m = list[1].toInt();

    return h * 60 + m;
}

void XFSpider::slotReloadWebpage()
{
    Debug::Printf("Reload bilibili webpage");
    this->Start();
}

void XFSpider::slotLoadWebpageFinished(bool ok)
{
    if( ok )
    {
        web_engine_page->toHtml([&](const QString& html){
            Debug::Printf("Load timeline data finished.");

            QFile file(PathHelper::GetXFDataDirPath() + "data.html");
            if( file.open(QFile::WriteOnly) ){
                file.write(html.toLocal8Bit());
                file.close();
            }
            /* 如果没有数据要更新，发出完成信号 */
            if( this->UpdateXFDatas(html) == false )
            {
                emit sigFinished();
            }
        });
    }
    else
    {
        Debug::Printf("Load Bilibili html faild.");

        /* 30 分钟后重新加载 */
        if( timer == nullptr ){
            timer = new QTimer(this);
            timer->setInterval(1000 * 60 * 30);

            connect(timer, SIGNAL(timeout()), this, SLOT(slotReloadWebpage()));
            Debug::Printf(QString::fromLocal8Bit("开启定时器，30 分钟后重新加载"));

            timer->start();
        }
    }
}

void XFSpider::slotNetworkFinished(QNetworkReply* reply)
{
    /* 下载发生错误，直接退出 */
    if( reply->error() != QNetworkReply::NoError )
    {
        this->DownloadError(reply);
    }
    else    /* 下载成功，执行相应操作 */
    {
        if( is_download_images )
        {
            /* 保存图片文件 */
            QImage image;
            image.loadFromData(reply->readAll());
            image.save(PathHelper::GetXFDataDirPath() + current_task_name + ".png");
        }
        else
        {
            XFDataMap::iterator it = xf_datas.find(current_task_name);
            if( it != xf_datas.end() )
            {
                PreviewData data = this->ParsePreviewHtml(reply->readAll());
                it.value().ExtendData = data;

                TaskData task_data;
                task_data.Name = current_task_name + QString::fromLocal8Bit("_封面");
                task_data.Url  = data.ImageUrl;
                image_download_queue << task_data;
            }
        }
    }
    reply->deleteLater();

    /* 下载完成，发出信号 */
    if( active_download_queue.isEmpty() )
    {
        Debug::Printf("Download Finsihed");
        if( is_download_images )
        {
            this->SaveXFDatas(xf_datas);
            emit sigFinished();
        }
        else
        {
            is_download_images = true;
            this->StartDownloadQueue(image_download_queue);
        }
    }
    else
    {
        /* 下载下一个任务 */
        this->DownloadQueueStep();
    }
}
