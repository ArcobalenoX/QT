#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QFile>
#include <QUrlQuery>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QBuffer>
#include <QFileDialog>
#include <QMessageBox>
#include <QByteArray>
#include <QSerialPort>        //提供访问串口的功能
#include <QSerialPortInfo>    //提供系统中存在的串口的信息

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    void serialPort_readyRead();
    ~Widget();
public slots:
    void radioBtnSlot();
    void boxChange();
    void boxChange2();

private:
    Ui::Widget *ui;

    QNetworkAccessManager *manager;

    //应答数据
    QByteArray replyData;

    //请求地址
    QString requestUrl;
    //Access Token
    QString accessToken;

    //图片路径
    QString imgPath;

    //创建串口对象
    QSerialPort serialPort;

    //设置串口名
    QByteArray byteArray;

    //识别模式，1串口，2为本地
    int Mode = 0;


    int flag;

private slots:

    void replyFinished(QNetworkReply *reply);

    void on_btnStartIdentify_clicked();

    void on_openImgBtn_clicked();
};

#endif // WIDGET_H
