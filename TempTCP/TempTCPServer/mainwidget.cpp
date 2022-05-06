#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), //chartView( new QChartView(this) )
    ui(new Ui::MainWidget)
{
    chartView = new QChartView(this);
    currentTempLabel = new QLabel(this);
    currentLCDNumber = new QLCDNumber(this);

    this->setGeometry(100,100,480,320);
    this->setMinimumSize(400,300);
    this->setMaximumSize(800,600);


    createChart();
    chartView->setGeometry(50,50,400,300);
    chartView->setChart(mainChart);

    currentTempLabel->setGeometry(50,20,200,30);
    currentTempLabel->setText("temp ");
    QFont font;
    font.setBold(true);
    font.setPointSize(20);
    currentTempLabel->setFont(font);
    currentTempLabel->setStyleSheet("color : red");

    currentLCDNumber->setGeometry(200,00,200,50);
    alertPlayer = new QMediaPlayer;
    //connect(alertPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64))); //监控进度条
    //alertPlayer->setMedia(QUrl::fromLocalFile("bgm.mp3"));
    alertPlayer->setMedia(QUrl("qrc:/bgm.mp3"));
    alertPlayer->setVolume(50);
    //alertPlayer->play();


    ui->setupUi(this);

    if(startServer())
    {
        qDebug()<<"wait connect...";
        connect(server,SIGNAL(newConnection()),this,SLOT(newConnectionAccept()));
    }
}

MainWidget::~MainWidget()
{
    delete ui;
}

bool MainWidget::startServer()
{
    server = new QTcpServer();
    QHostAddress addr("127.0.0.1");
    return server->listen(addr,8888);
}
void MainWidget::newConnectionAccept()
{
    qDebug()<<"new connect";
    //获取当前传输的套接字 进行数据收发
    socket = server->nextPendingConnection();
    socket->write("connecting...");
    connect(socket,SIGNAL(readyRead()),this,SLOT(read_data()));
}

void MainWidget::read_data()
{
        short temp = 0;
        float Rtemp=0;
        qreal passMinute = QTime::currentTime().minute();
        socket->read((char*)&temp,sizeof (temp));
        Rtemp = (temp>>8) + (temp & 0x00FF)/100.0 ;
        qDebug()<<Rtemp;
        //按分钟刷新界面
        if(passMinute != QTime::currentTime().minute())
        {
            dotSeries->clear();
            connectLineSeries->clear();
            passMinute = QTime::currentTime().minute();
        }

        dotSeries->append(QTime::currentTime().second(),Rtemp);
        connectLineSeries->append(QTime::currentTime().second(),Rtemp);


        QString tempstr("current temp: ");
        tempstr += QString("%1").arg(Rtemp);
        currentTempLabel->setText(tempstr);
        currentLCDNumber->display(tempstr);

        //声音报警
        if(Rtemp>37.5)
        {
            if(alertPlayer->state() == QMediaPlayer :: PlayingState)
            {
                alertPlayer->stop();
            }
            alertPlayer->play();
        }
        else
        {
            alertPlayer->stop();
        }
}


void MainWidget::createChart()
{
    //配置基础的图表数据
    mainChart = new QChart();
    dotSeries = new QScatterSeries;
    lineSeries = new QLineSeries;
    connectLineSeries = new QLineSeries;

    //X Y 坐标系
    QValueAxis* axisX,*axisY;
    axisX = new QValueAxis;
    axisY = new QValueAxis;
    //数值范围
    axisX->setRange(0,60);
    axisY->setRange(34,44);
    //数值形式
    axisX->setLabelFormat("%.2f");
    axisY->setLabelFormat("%.2f");
    //表格分割虚线
    axisX->setGridLineVisible(true);
    axisY->setGridLineVisible(true);
    //分割格子数
    axisX->setTickCount(7);
    axisX->setMinorTickCount(1);
    axisY->setTickCount(10);
    axisY->setMinorTickCount(1);
    //坐标轴含义
    axisX->setTitleText("time");
    axisY->setTitleText("temp");

    axisY->setLinePenColor(Qt::blue);

    mainChart->addAxis(axisX,Qt::AlignBottom);
    mainChart->addAxis(axisY,Qt::AlignLeft);


    mainChart->addSeries(connectLineSeries);
    mainChart->addSeries(lineSeries);
    mainChart->addSeries(dotSeries);

    //mainChart->legend()->hide();  //图例
    //绑定坐标系和数值范围，必须先 mainChart->addSeries(dotSeries);
    dotSeries->attachAxis(axisX);
    dotSeries->attachAxis(axisY);
    //dotSeries->append(10,37);

    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);
    lineSeries->setColor(Qt::red);
    lineSeries->setPen(QPen(Qt::red,5,Qt::DashLine));

    connectLineSeries->attachAxis(axisX);
    connectLineSeries->attachAxis(axisY);


    lineSeries->append(0,37.5);
    lineSeries->append(60,37.5);





}
