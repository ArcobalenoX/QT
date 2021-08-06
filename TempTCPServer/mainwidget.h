#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include<QTcpServer>
#include<QTcpSocket>
#include<QDebug>
#include<QLabel>
#include<QChart>
#include<QtCharts>
#include<QtCharts/QScatterSeries>
#include<QtCharts/QValueAxis>
#include<QtCharts/QLineSeries>
#include<QPen>
#include<QChartView>
#include<QLCDNumber>
#include<QMediaPlayer>
using namespace QtCharts;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    QTcpServer *server;
    QTcpSocket *socket;
    //图表视图
    QChartView * chartView;
    //主图表
    QChart* mainChart;
    //图类型 点状图
    QScatterSeries* dotSeries;

    //图类型 折线图
    QLineSeries * lineSeries;
    QLineSeries * connectLineSeries;

    //标签
    QLabel *currentTempLabel;

    //LCD示值
    QLCDNumber * currentLCDNumber;

    //音视频
    QMediaPlayer * alertPlayer;

    bool startServer();
    void createChart();

private slots:
    void newConnectionAccept();

private:
    Ui::MainWidget *ui;
};
#endif // MAINWIDGET_H
