#include "widget.h"
#include "ui_widget.h"
#include <QDir>  //头文件
#include <QImage>
#include "QIcon"

using namespace Qt;

Widget::Widget(QWidget *parent) :QWidget(parent),ui(new Ui::Widget)
{
    ui->setupUi(this);
    requestUrl = "https://aip.baidubce.com/rest/2.0/image-classify/v1/currency";
    connect(ui->radioButton, SIGNAL(toggled(bool)), this, SLOT(radioBtnSlot()));
    imgPath = "";
    manager = new QNetworkAccessManager(this);

    QString fileName = "./Token.txt";                     //获取保存的Token
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "No Fail!" << endl;
    }
    QTextStream out(&file);
    ui->textEdit->setText(out.readLine());
    file.close();

    //设置数据位数
    serialPort.setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    serialPort.setParity(QSerialPort::NoParity);
    //设置停止位
    serialPort.setStopBits(QSerialPort::OneStop);
    //设置流控制
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    QObject::connect(&serialPort, &QSerialPort::readyRead, this, &Widget::serialPort_readyRead);
    QObject::connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(boxChange()));
    QObject::connect(ui->comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(boxChange2()));
}

void Widget::boxChange()      //槽函数获取当前端口号
{
    if(ui->comboBox->currentText().isEmpty()) return;
    serialPort.close();
    serialPort.setPortName(ui->comboBox->currentText());
    serialPort.open(QIODevice::ReadWrite);
}

void Widget::boxChange2()
{
    if(ui->comboBox->currentText().isEmpty()) return;
    serialPort.close();
    serialPort.setBaudRate(ui->comboBox_2->currentText().toInt());
    serialPort.open(QIODevice::ReadWrite);
}

void Widget::radioBtnSlot()
{
    if (ui->radioButton->isChecked())
    {
        foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            QSerialPort serial;
            serial.setPort(info);
            if(serial.open(QIODevice::ReadWrite))
            {
                ui->comboBox->addItem(serial.portName());
                serial.close();
            }
        }
        ui->comboBox_2->addItem("2400");
        ui->comboBox_2->addItem("4800");
        ui->comboBox_2->addItem("9600");
        ui->comboBox_2->addItem("38400");
        ui->comboBox_2->addItem("115200");
        ui->comboBox_2->addItem("921600");
        //打开串口
        serialPort.open(QIODevice::ReadWrite);
        Mode = 1;
    }
    else
    {
        ui->comboBox->clear();
        ui->comboBox_2->clear();
        //关闭串口
        serialPort.close();
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::serialPort_readyRead()   //串口数据接收
{
    //从接收缓冲区中读取数据
    static QByteArray temp;
    temp = serialPort.readAll();
    byteArray.append(temp);
    if(!temp.isEmpty())
    {
        if(temp.contains("###"))
        {
            byteArray =  byteArray.right(byteArray.length()-byteArray.indexOf('\xff'));
            byteArray =  byteArray.left(byteArray.lastIndexOf('\xd9')+1);
            QPixmap imageresult;
            imageresult.loadFromData(byteArray);
            imageresult.save("./result.jpg");
            flag = 1;
            Widget::on_btnStartIdentify_clicked();
            flag = 0;

            /*       调试用代码
            QString fileName = "./txt1.txt";
            QFile file(fileName);
            if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Truncate))
           {
                qDebug() << "open fail!" << endl;
           }
            QTextStream in(&file);
            in<<byteArray.toHex();
            file.close();
        */
            byteArray.clear();
        }

    }
}

void Widget::replyFinished(QNetworkReply *reply)     //百度云回传数据解析,不会搞
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(200 != statusCode)
    {
        qDebug() << "Status code is error:" << statusCode;
        return;
    }
    replyData = reply->readAll();
    //    qDebug()<<"reply data is:"<<QString(replyData);

    QJsonParseError json_error;
    QJsonDocument document = QJsonDocument::fromJson(replyData, &json_error);
    if(json_error.error == QJsonParseError::NoError)
    {
        //判断是否是对象,然后开始解析数据
        if(document.isObject())
        {
            QJsonObject obj = document.object();
            //解析反馈结果
            QString buf = QString(document.toJson());

            QString name = buf.section("\"",7,7);

            qDebug() << "果蔬识别结果为："  <<endl;

            qDebug() << name;
            ui->lblName->setText(name);

        }
        //保存Token数据
        QString fileName = "./Token.txt";
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            qDebug() << "open fail!" << endl;
        }
        QTextStream in(&file);
        in<<ui->textEdit->toPlainText();
        file.close();

    }
    reply->deleteLater();
}

void Widget::on_btnStartIdentify_clicked()    //整合数据，发送https请求
{
    //设置请求地址
    accessToken = ui->textEdit->toPlainText();
    QUrl url(requestUrl + "?access_token=" + accessToken);
    QNetworkRequest request(url);

    //设置数据提交格式，这个不能自己随便写，每个平台的格式可能不一样，百度AI要求的格式为application/json
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    QString imgData_S;
    if(flag)
    {
        imgData_S = QString(byteArray.toBase64());
        QImage img("./result.jpg");
        //保持长宽比进行缩放
        img = img.scaled(ui->lblImg->width(), ui->lblImg->height(), Qt::KeepAspectRatio);
        ui->lblImg->setPixmap(QPixmap::fromImage(img));
    }
    else
    {
        //将要检测的图片进行BASE64编码
        QImage image(imgPath);
        QByteArray ba;
        QBuffer buffer(&ba);
        buffer.open(QIODevice::WriteOnly);
        //以png格式将图片数据写入ba
        image.save(&buffer, "png");
        //将图片进行BASE64编码
        imgData_S = QString(ba.toBase64());
        buffer.close();
    }

    QByteArray imgData_A  = QUrl::toPercentEncoding(imgData_S);
    imgData_A.append("&top_num:1");
    imgData_A.insert(0,QByteArray("image="));


    //发送请求
    manager->post(request,imgData_A);

}

void Widget::on_openImgBtn_clicked()     //本地图片加载
{
    ui->lblImg->clear();
    ui->lblName->clear();

    imgPath = QFileDialog::getOpenFileName(this, tr("Open Image"),
                                           "./", tr("Image files(*.jpg *.png *.jpeg);;All files (*.*)"));
    if(imgPath.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.warning(this,"警告","打开图片失败!");
        return;
    }
    else
    {
        QImage img(imgPath);
        //保持长宽比进行缩放
        img = img.scaled(ui->lblImg->width(), ui->lblImg->height(), Qt::KeepAspectRatio);
        ui->lblImg->setPixmap(QPixmap::fromImage(img));
        Mode = 2;
    }
}
