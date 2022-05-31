#include "weiqiantools.h"
#include <QTabWidget>
#include <QGridLayout>
#include <QFile>
#include <QStyleFactory>

#include "serialportwidget.h"
#include "canwidget.h"
#include "otherswidget.h"
#include "watchdogwidget.h"
#include <QLayout>

WeiqianTools::WeiqianTools(QWidget *parent): QDialog(parent)
{
    SerialPortWidget    *pSerialPortWidget      = new SerialPortWidget;
    CanWidget           *pCanWidget             = new CanWidget;
    OthersWidget        *pBackLightWidget       = new OthersWidget;
    WatchDogWidget      *pWatchDogWidget        = new WatchDogWidget;


    QTabWidget *pTextTab = new QTabWidget;

    pTextTab->addTab(pSerialPortWidget, tr("串口测试"));
    pTextTab->addTab(pCanWidget,        tr("CAN测试"));
    pTextTab->addTab(pWatchDogWidget,   tr("看门狗测试"));
    pTextTab->addTab(pBackLightWidget,  tr("其他测试"));

    pTextTab->resize(this->size());

    QGridLayout *pMainLayout = new QGridLayout;
    pMainLayout->addWidget(pTextTab, 0, 0, 1, 1);
    this->setLayout(pMainLayout);

}

WeiqianTools::~WeiqianTools()
{

}


