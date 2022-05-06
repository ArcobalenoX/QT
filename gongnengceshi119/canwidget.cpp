#include "canwidget.h"

#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTimer>
#include <QGridLayout>
#include <QString>
#include <QList>
#include <QTimer>
#include <QTextEdit>
#include <unistd.h>
#include <QListView>

#include "weiqianfunctions.h"
#include "canreadthread.h"
#include "com.h"

CanWidget::CanWidget(QWidget *parent) : QWidget(parent)
{
    im = new InputMethod;
      cansentcont=0;
      canreceivecont=0  ;
    QLabel          *pBaudLabel     = new QLabel;
    QLabel          *pCanIdLabel    = new QLabel;
    QLabel			*pSentCountLabel			    = new QLabel;
    QLabel			*pRecevCountLabel			    = new QLabel;
    QPushButton     *pOpenBut       = new QPushButton;
    QPushButton     *pCloseBut      = new QPushButton;
    pSentBut       = new QPushButton;
    QPushButton     *pClearBut      = new QPushButton;

    m_pAutoRadioBut   = new QCheckBox;
    m_pAutoTimer      =new QTimer;
    m_pTimeEdit             = new QLineEdit;
    m_pSentCountEdit        = new QLineEdit;
    m_pReciveCountEdit      = new QLineEdit;
    m_pReceiveTimer			= new QTimer;



                    m_pReceiveEdit  = new QTextEdit;
                    m_pBaudCombo    = new QComboBox;
                    m_pCanIdEdit    = new QLineEdit;
                    m_pSendEdit     = new QLineEdit;
                    m_pCanIdEdit->installEventFilter(im);
                    m_pCanIdEdit->setObjectName("m_pCanIdEdit");
                    m_pSendEdit->installEventFilter(im);
                    m_pSendEdit->setObjectName("m_pSendEdit");

                    connect(im,SIGNAL(setImObjName(QString)),this,SLOT(createImConnect(QString)));//connect the input method

                    m_pTimeEdit->installEventFilter(im);
                    m_pTimeEdit->setObjectName("m_pTimeEdit");
                    connect(im,SIGNAL(setImObjName(QString)),this,SLOT(createImConnect(QString)));//connect the input method

    pBaudLabel      ->setText(tr("CAN波特率："));
    pCanIdLabel     ->setText(tr("帧ID："));
    pOpenBut        ->setText(tr("打开"));
    pCloseBut       ->setText(tr("关闭"));
    pSentBut        ->setText(tr("发送"));
    pClearBut       ->setText(tr("清空"));
     m_pAutoRadioBut        ->setText(tr("自动发送周期 (ms) : "));

    QList<QString> baudList;
    baudList    <<"50000"
                <<"100000"
                <<"125000"
                <<"250000";

    m_pBaudCombo->addItems(baudList);
    m_pBaudCombo->setView(new QListView());
    m_pBaudCombo->setCurrentIndex(3);

    pBaudLabel			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pCanIdLabel			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pOpenBut			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pCloseBut			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pSentBut			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pClearBut			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pReceiveEdit		->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pBaudCombo		->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pCanIdEdit		->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pSendEdit			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pAutoRadioBut     ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pSentCountLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_pAutoRadioBut        ->setText(tr("自动发送周期 (ms) : "));
    pSentCountLabel     ->setText(tr("发送次数"));
    pRecevCountLabel    ->setText(tr("接收次数"));

    QHBoxLayout * subHLayout1 = new QHBoxLayout;
    QHBoxLayout * subHLayout2 = new QHBoxLayout;
    QVBoxLayout * subVLayout = new QVBoxLayout;
    subHLayout1->addWidget(m_pAutoRadioBut);

    subHLayout1->addWidget(m_pTimeEdit);
    m_pTimeEdit->setMaximumSize(120,50);
     //   subHLayout1->addWidget(pMsLabel);

    subHLayout2->addWidget(pSentCountLabel);
    subHLayout2->addWidget(m_pSentCountEdit);
    m_pSentCountEdit->setMaximumSize(120,50);
    m_pSentCountEdit->setEnabled(false);
    subHLayout2->addWidget(pRecevCountLabel);
    subHLayout2->addWidget(m_pReciveCountEdit);
    m_pReciveCountEdit->setMaximumSize(120,50);
    m_pReciveCountEdit->setEnabled(false);


    subVLayout->addLayout(subHLayout1);
    subVLayout->addLayout(subHLayout2);
    QGridLayout		*pMainLayout		= new QGridLayout;
    pMainLayout->addWidget(pBaudLabel,			0, 0, 1, 2);
    pMainLayout->addWidget(m_pBaudCombo,		1, 0, 1, 2);
    pMainLayout->addWidget(pCanIdLabel,         2, 0, 1, 2);
    pMainLayout->addWidget(m_pCanIdEdit,        3, 0, 1, 2);
    pMainLayout->addWidget(pOpenBut,            4, 0, 1, 1);
    pMainLayout->addWidget(pCloseBut,           4, 1, 1, 1);
    pMainLayout->addWidget(m_pReceiveEdit,		0, 2, 4, 8);
    pMainLayout->addWidget(m_pSendEdit,			4, 2, 1, 8);

    pMainLayout->addLayout(subVLayout,		    5, 0, 1, 6);

    pMainLayout->addWidget(pClearBut,           5, 7, 1, 1);
    pMainLayout->addWidget(pSentBut,			5, 8, 1, 1);

    this->setLayout(pMainLayout);

    connect(pOpenBut,       SIGNAL(clicked()),  this,   SLOT(openCan()));
    connect(pCloseBut,      SIGNAL(clicked()),  this,   SLOT(closeCan()));
    connect(pClearBut,      SIGNAL(clicked()),  this,   SLOT(clear()));
    connect(pSentBut,       SIGNAL(clicked()),  this,   SLOT(send()));

    m_pCanIdEdit->setText("1");
    m_pSendEdit->setText("12345678");
    m_pTimeEdit->setText(tr("300"));
    m_pSentCountEdit->setText(tr("0"));
    m_pReciveCountEdit->setText(tr("0"));


    m_pThread = new CanReadThread();
    connect(m_pThread, SIGNAL(receiveSignal(int,QString,int)), this, SLOT(receiveData(int,QString,int)));
    connect(m_pAutoRadioBut,    SIGNAL(clicked()),     this,   SLOT(Autohave()));
    connect(m_pAutoTimer,      SIGNAL(timeout()),     this,   SLOT(timeAuto()));

}

void CanWidget::Autohave()
{

    if(m_pAutoRadioBut->isChecked()) {
        tmiems=m_pTimeEdit->text().toInt();
        m_pAutoTimer->start(tmiems);
        pSentBut->setEnabled(false);
    }else{
        m_pAutoTimer->stop();
        pSentBut->setEnabled(true);
    }
}

void CanWidget::timeAuto()
{
   // if(haveopen)
    send();
    if(cansentcont<18446744073709551610)
    cansentcont++;
    else
        cansentcont=0;
    m_pSentCountEdit->setText(QString::number(cansentcont));
}

void CanWidget::createImConnect(QString ObjName)
{
    disconnect(im->keyboard,SIGNAL(setvalue(QString)),m_pSendEdit,SLOT(setText(QString)));
    disconnect(im->keyboard,SIGNAL(setvalue(QString)),m_pCanIdEdit,SLOT(setText(QString)));
    disconnect(im->keyboard,SIGNAL(setvalue(QString)),m_pTimeEdit,SLOT(setText(QString)));
    if(ObjName=="m_pSendEdit")
        connect(im->keyboard,SIGNAL(setvalue(QString)),m_pSendEdit,SLOT(setText(QString)));
    if(ObjName=="m_pCanIdEdit")
        connect(im->keyboard,SIGNAL(setvalue(QString)),m_pCanIdEdit,SLOT(setText(QString)));
    if(ObjName=="m_pTimeEdit")
        connect(im->keyboard,SIGNAL(setvalue(QString)),m_pTimeEdit,SLOT(setText(QString)));

}

void CanWidget::openCan()
{
    system("ifconfig -a | grep can >/tmp/HAVEcan.dat");
    QFile filetmpwlan("/tmp/HAVEcan.dat");
    filetmpwlan.open(QFile::ReadOnly);
    QString stmpwlan =filetmpwlan.readAll();
    QString stmp2wlan="can";
    if(!stmpwlan.contains(stmp2wlan) ){
        //"No have can");

        QDialog * setdialog = new QDialog;
        QLabel * settmp = new QLabel;
        settmp->setText(tr("标配版没有CAN模块"));
        settmp->setStyleSheet("color:red;");
        QTimer*timer = new QTimer(setdialog);
        timer->start(600);
        connect(timer,SIGNAL(timeout()),setdialog,SLOT(close()));
        QGridLayout *tmplayout = new QGridLayout;
        tmplayout->addWidget(settmp,0,0,1,1);
        setdialog->setLayout(tmplayout);
        setdialog->setWindowFlags(Qt::FramelessWindowHint);
        setdialog->exec();
    }
    WeiqianFunctions::InitCan(m_pBaudCombo->currentText().toInt());
    if(WeiqianFunctions::OpenCan() > 0) {
        m_pThread->start();
    }
}

void CanWidget::closeCan()
{
    if(m_pThread->isRunning()) {
        m_pThread->stop();
        usleep(100000);
    }
    WeiqianFunctions::CloseCan();

}

void CanWidget::clear()
{
    m_pReceiveEdit->setText("");
    m_pSendEdit->setText("");
    receivecont=0;
    m_pReciveCountEdit->setText("0");
    cansentcont=0;
    m_pSentCountEdit->setText("0");
}

void CanWidget::send()
{

    if(m_pSendEdit->text().count() > 0) {

        QByteArray temp = m_pSendEdit->text().toUtf8();
        const char *pBuff = temp.data();
       WeiqianFunctions::CanWrite(m_pCanIdEdit->text().toInt(), pBuff,(m_pSendEdit->text().length()));
    } else {
        if(1){
            char *pBuffcrl = new char[8];
            pBuffcrl[0]=0x01;
            pBuffcrl[1]=0x02;
            pBuffcrl[2]=0x03;
            pBuffcrl[3]=0x00;
            pBuffcrl[4]=0x05;
            pBuffcrl[5]=0x00;
            pBuffcrl[6]=0x07;
            WeiqianFunctions::CanWrite(m_pCanIdEdit->text().toInt(), pBuffcrl,8);
            delete pBuffcrl;
        }
    }
}

void CanWidget::receiveData(const int id, const QString data, const int lenght)
{
      m_pReceiveEdit->append("Id:" + QString::number(id) + "  Data:" + data);
      if(receivecont<18446744073709551610)
      receivecont++;
      else
         receivecont=0;
    //  m_pReciveCountEdit->setText("");
    //  QString tmp =QString.toInt(receivecont);
      m_pReciveCountEdit->setText(QString::number(receivecont));
}

