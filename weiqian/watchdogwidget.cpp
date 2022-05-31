#include "watchdogwidget.h"

#include <QGroupBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>

#include "weiqianfunctions.h"

WatchDogWidget::WatchDogWidget(QWidget *parent) : QWidget(parent)
{
    im = new InputMethod;
    QGroupBox       *pEnableGroupBox    = new QGroupBox;
    QGroupBox       *pOpenGroupBox      = new QGroupBox;
    QGroupBox       *pSetTimeGroupBox   = new QGroupBox;
    QGroupBox       *pGetTimeGroupBox   = new QGroupBox;
    QGroupBox       *pFeedGroupBox      = new QGroupBox;

                    m_pEnableRadioBut   = new QRadioButton;
                    m_pDisableRadioBut  = new QRadioButton;
                    m_pOpenRadioBut     = new QRadioButton;
                    m_pCloseRadioBut    = new QRadioButton;


    QHBoxLayout     *pEnableLayout      = new QHBoxLayout;
    QHBoxLayout     *pOpenLayout        = new QHBoxLayout;
    QHBoxLayout     *pSetTimeLayout     = new QHBoxLayout;
    QHBoxLayout     *pGetTimeLayout     = new QHBoxLayout;
    QHBoxLayout     *pFeedLayout        = new QHBoxLayout;

    QPushButton     *pSetTimeBut        = new QPushButton;
    QPushButton     *pGetTimeBut        = new QPushButton;
    QPushButton     *pFeedBut           = new QPushButton;

    QGridLayout     *pMainLayout        = new QGridLayout;

                    m_pGetTimeLabel     = new QLabel;
                    m_pCountDownLabel   = new QLabel;
                    m_pSetTimeEdit      = new QLineEdit;

                    m_pCountDownTimer   = new QTimer;


    pEnableGroupBox     ->setTitle(tr("看门狗使能"));
    pOpenGroupBox       ->setTitle(tr("看门狗状态"));
    pSetTimeGroupBox    ->setTitle(tr("设置看门狗间隔时间"));
    pGetTimeGroupBox    ->setTitle(tr("获取看门狗间隔时间"));
    pFeedGroupBox       ->setTitle(tr("喂狗"));

    m_pEnableRadioBut     ->setText(tr(" 看门狗使能 "));
    m_pDisableRadioBut    ->setText(tr("看门狗使不能"));
    m_pOpenRadioBut       ->setText(tr("看门狗已启动"));
    m_pCloseRadioBut      ->setText(tr("看门狗已停止"));

    pSetTimeBut         ->setText(tr("设置时间"));
    pGetTimeBut         ->setText(tr("获取时间"));
    pFeedBut            ->setText(tr("喂狗"));

    m_pGetTimeLabel     ->setText(tr("看门狗时间间隔：　　　　　"));
    m_pCountDownLabel   ->setText(tr("看门狗倒计时：　　　　　"));
    m_pSetTimeEdit      ->setText(tr("10"));

    m_pSetTimeEdit->installEventFilter(im);
    m_pSetTimeEdit->setObjectName("m_pSetTimeEdit");
    connect(im,SIGNAL(setImObjName(QString)),this,SLOT(createImConnect(QString)));//connect the input method


    pSetTimeLayout->addWidget(m_pSetTimeEdit);
    pSetTimeLayout->addWidget(pSetTimeBut);

    pGetTimeLayout->addWidget(m_pGetTimeLabel);
    pGetTimeLayout->addWidget(pGetTimeBut);

    pFeedLayout->addWidget(pFeedBut);
    pFeedLayout->addWidget(m_pCountDownLabel);

    pEnableLayout->addWidget(m_pEnableRadioBut);
    pEnableLayout->addWidget(m_pDisableRadioBut);

    pOpenLayout->addWidget(m_pOpenRadioBut);
    pOpenLayout->addWidget(m_pCloseRadioBut);


    m_pSetTimeEdit      ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pSetTimeBut			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pGetTimeLabel     ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pGetTimeBut			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pFeedBut			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pCountDownLabel   ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pEnableRadioBut   ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pDisableRadioBut  ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pOpenRadioBut     ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pCloseRadioBut    ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pEnableGroupBox     ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pOpenGroupBox       ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pSetTimeGroupBox    ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pGetTimeGroupBox    ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pFeedGroupBox       ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    pEnableGroupBox     ->setLayout(pEnableLayout);
    pOpenGroupBox       ->setLayout(pOpenLayout);
    pSetTimeGroupBox    ->setLayout(pSetTimeLayout);
    pGetTimeGroupBox    ->setLayout(pGetTimeLayout);
    pFeedGroupBox       ->setLayout(pFeedLayout);

    pMainLayout->addWidget(pEnableGroupBox,     0,0,1,1);
    pMainLayout->addWidget(pOpenGroupBox,       0,1,1,1);
    pMainLayout->addWidget(pSetTimeGroupBox,    1,0,1,1);
    pMainLayout->addWidget(pGetTimeGroupBox,    1,1,1,1);
    pMainLayout->addWidget(pFeedGroupBox,       2,0,1,2);
    this->setLayout(pMainLayout);

    connect(m_pEnableRadioBut,      SIGNAL(clicked()),     this,   SLOT(watchdogEnable()));
    connect(m_pDisableRadioBut,     SIGNAL(clicked()),     this,   SLOT(watchdogDisable()));
    connect(m_pOpenRadioBut,        SIGNAL(clicked()),     this,   SLOT(watchdogOpen()));
    connect(m_pCloseRadioBut,       SIGNAL(clicked()),     this,   SLOT(watchdogClose()));
    connect(pSetTimeBut,            SIGNAL(clicked()),     this,   SLOT(watchdogSetTime()));
    connect(pGetTimeBut,            SIGNAL(clicked()),     this,   SLOT(watchdogGetTime()));
    connect(pFeedBut,               SIGNAL(clicked()),     this,   SLOT(watchdogFeedDog()));
    connect(m_pCountDownTimer,      SIGNAL(timeout()),     this,   SLOT(watchdogCountDown()));


    m_pEnableRadioBut->setChecked(true);
    m_pCloseRadioBut->setChecked(true);

    m_count = 0;
}

void WatchDogWidget::createImConnect(QString ObjName)
{
    disconnect(im->keyboard,SIGNAL(setvalue(QString)),m_pSetTimeEdit,SLOT(setText(QString)));
    if(ObjName=="m_pSetTimeEdit")
        connect(im->keyboard,SIGNAL(setvalue(QString)),m_pSetTimeEdit,SLOT(setText(QString)));;
}

void WatchDogWidget::watchdogEnable()
{
    WeiqianFunctions::WatchDogEnable(true);
    m_pDisableRadioBut->setChecked(false);
    m_pCountDownTimer->start(1000);
}

void WatchDogWidget::watchdogDisable()
{
    WeiqianFunctions::WatchDogEnable(false);
    m_pEnableRadioBut->setChecked(false);
    m_pCountDownTimer->stop();
}

void WatchDogWidget::watchdogOpen()
{
    WeiqianFunctions::StarWatchDog();
    m_pCloseRadioBut->setChecked(false);
    m_count = WeiqianFunctions::GetWatchDog();
    m_pCountDownTimer->start(1000);
}

void WatchDogWidget::watchdogClose()
{
    WeiqianFunctions::StopWatchDog();
    m_pOpenRadioBut->setChecked(false);
    m_pCountDownTimer->stop();
}

void WatchDogWidget::watchdogSetTime()
{
    WeiqianFunctions::SetWatchDog(m_pSetTimeEdit->text().toInt());
}

void WatchDogWidget::watchdogGetTime()
{
    m_pGetTimeLabel->setText(tr("看门狗时间间隔：　") + QString::number(WeiqianFunctions::GetWatchDog()) + "   ");
}

void WatchDogWidget::watchdogFeedDog()
{
    WeiqianFunctions::FeedWatchDog();
    m_count = WeiqianFunctions::GetWatchDog();
}

void WatchDogWidget::watchdogCountDown()
{
    m_count --;
    m_pCountDownLabel->setText(tr("看门狗倒计时：　") + QString::number(m_count) + "　　　");
}

