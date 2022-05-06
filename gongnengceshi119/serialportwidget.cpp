#include "serialportwidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QString>
#include <QTimer>
#include <QList>
#include <unistd.h>
#include <QTextEdit>
#include <QListView>

#include "serialport.h"
#include "serialportreadthread.h"
#include "com.h"

SerialPortWidget::SerialPortWidget(QWidget *parent) : QWidget(parent)
{
    im = new InputMethod;
      sentcont=0;
      receivecont=0  ;
    QLabel			*pNameLabel1			= new QLabel;
    QLabel			*pBaudrateLabel			= new QLabel;
    QLabel			*pDataBitsLabel			= new QLabel;
    QLabel			*pParityLable			= new QLabel;
    QLabel			*pStopBitsLabel			= new QLabel;
    QLabel			*pMsLabel			    = new QLabel;
    QLabel			*pSentCountLabel			    = new QLabel;
    QLabel			*pRecevCountLabel			    = new QLabel;



    QPushButton		*pOpenButton			= new QPushButton;
    pSentButton			= new QPushButton;
    QPushButton		*pCloseButton			= new QPushButton;
    QPushButton		*pCleanButton			= new QPushButton;
    m_pAutoRadioBut   = new QCheckBox;
    m_pAutoTimer      =new QTimer;
                    m_pReceiveEdit			= new QTextEdit;
                    m_pSendEdit				= new QLineEdit;
                    m_pTimeEdit             = new QLineEdit;
                    m_pSentCountEdit        = new QLineEdit;
                    m_pReciveCountEdit      = new QLineEdit;
                    m_pReceiveTimer			= new QTimer;
                    m_pBandrateCombo		= new QComboBox;
                    m_pDataBitsCombo		= new QComboBox;
                    m_pParityCombo			= new QComboBox;
                    m_pStopBitsCombo		= new QComboBox;
                    m_pNameCombo            = new QComboBox;
                    m_pSendEdit->installEventFilter(im);
                    m_pSendEdit->setObjectName("m_pSendEdit");
                    connect(im,SIGNAL(setImObjName(QString)),this,SLOT(createImConnect(QString)));//connect the input method

                    m_pTimeEdit->installEventFilter(im);
                    m_pTimeEdit->setObjectName("m_pTimeEdit");
                    connect(im,SIGNAL(setImObjName(QString)),this,SLOT(createImConnect(QString)));//connect the input method


    pNameLabel1			->setText(tr("串  口："));
    pBaudrateLabel		->setText(tr("波特率："));
    pDataBitsLabel		->setText(tr("数据位："));
    pParityLable		->setText(tr("校验位："));
    pStopBitsLabel		->setText(tr("停止位："));
    pMsLabel            ->setText("ms");
    pSentCountLabel     ->setText(tr("发送次数"));
    pRecevCountLabel    ->setText(tr("接收次数"));

    pOpenButton			->setText(tr("打  开"));
    pSentButton			->setText(tr("发  送"));
    pCloseButton		->setText(tr("关  闭"));
    pCleanButton		->setText(tr("清  空"));
    m_pAutoRadioBut        ->setText(tr("自动发送周期 (ms) : "));


    QList<QString>	bandrateList;
    bandrateList	<< "4800"
                    << "9600"
                    << "38400"
                    << "57600"
                    << "115200" ;

    QList<QString>	dataBitsList;
    dataBitsList	<< "5"
                    << "6"
                    << "7"
                    << "8";

    QList<QString>	parityList;
    parityList		<< "N"
                    << "E"
                    << "O"
                    << "S";

    QList<QString>	stopBitsList;
    stopBitsList	<< "1"
                    << "1.5"
                    << "2";

    QList<QString>  nameList;
    nameList        << "COM1"
                    << "COM2"
                    << "COM3"
                    << "COM4";

    m_pBandrateCombo->addItems(bandrateList);
    m_pBandrateCombo->setView(new QListView());
    m_pDataBitsCombo->addItems(dataBitsList);
    m_pDataBitsCombo->setView(new QListView());
    m_pParityCombo->addItems(parityList);
    m_pParityCombo->setView(new QListView());
    m_pStopBitsCombo->addItems(stopBitsList);
    m_pStopBitsCombo->setView(new QListView());
    m_pNameCombo->addItems(nameList);
    m_pNameCombo->setView(new QListView());


    pNameLabel1			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBaudrateLabel		->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pDataBitsLabel		->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pParityLable		->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pStopBitsLabel		->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pBandrateCombo	->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pDataBitsCombo	->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pNameCombo        ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pParityCombo		->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pStopBitsCombo	->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pOpenButton			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pSentButton			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pCloseButton		->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pCleanButton		->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pReceiveEdit		->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pSendEdit			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
   // m_pTimeEdit         ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pAutoRadioBut     ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(pOpenButton,		SIGNAL(clicked()), this, SLOT(openSerialPort()));
    connect(pSentButton,		SIGNAL(clicked()), this, SLOT(sendData()));
    connect(pCloseButton,		SIGNAL(clicked()), this, SLOT(closeSerialPort()));
    connect(pCleanButton,		SIGNAL(clicked()), this, SLOT(cleanReceiveLabel()));
    connect(m_pAutoRadioBut,    SIGNAL(clicked()),     this,   SLOT(Autohave()));

    QGridLayout		*pMainLayout		= new QGridLayout;
    pMainLayout->addWidget(pNameLabel1,			0, 0, 1, 1);
    pMainLayout->addWidget(m_pNameCombo,		0, 1, 1, 2);
    pMainLayout->addWidget(pBaudrateLabel,		1, 0, 1, 1);
    pMainLayout->addWidget(m_pBandrateCombo,	1, 1, 1, 2);
    pMainLayout->addWidget(pDataBitsLabel,		2, 0, 1, 1);
    pMainLayout->addWidget(m_pDataBitsCombo,	2, 1, 1, 2);
    pMainLayout->addWidget(pParityLable,		3, 0, 1, 1);
    pMainLayout->addWidget(m_pParityCombo,		3, 1, 1, 2);
    pMainLayout->addWidget(pStopBitsLabel,		4, 0, 1, 1);
    pMainLayout->addWidget(m_pStopBitsCombo,	4, 1, 1, 2);

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

    pMainLayout->addLayout(subVLayout,		    5, 0, 1, 3);
 //subVLayout   pMainLayout->addWidget(m_pAutoRadioBut,		5, 0, 1, 1);
 //   pMainLayout->addWidget(m_pTimeEdit,		    5, 1, 1, 1);
//    pMainLayout->addWidget(pMsLabel,		    5, 2, 1, 1);

    pMainLayout->addWidget(m_pReceiveEdit,		0, 3, 4, 8);
    pMainLayout->addWidget(m_pSendEdit,			4, 3, 1, 8);
    pMainLayout->addWidget(pOpenButton,			5, 3, 1, 1);
    pMainLayout->addWidget(pCloseButton,		5, 4, 1, 1);
    pMainLayout->addWidget(pCleanButton,		5, 8, 1, 1);
    pMainLayout->addWidget(pSentButton,			5, 9, 1, 1);

    this->setLayout(pMainLayout);

    m_pMySerialPort = new SerialPort();
    m_pReadThread = new SerialPortReadThread();

    connect(m_pReadThread, SIGNAL(receiveSignal(QString)), this, SLOT(receiveData(QString)));
    connect(m_pAutoTimer,      SIGNAL(timeout()),     this,   SLOT(timeAuto()));

    m_pBandrateCombo->setCurrentIndex(4);
    m_pDataBitsCombo->setCurrentIndex(3);
    m_pNameCombo->setCurrentIndex(1);
    m_pSendEdit->setText(tr("weiqian "));
    m_pTimeEdit->setText(tr("300"));
    m_pSentCountEdit->setText(tr("0"));
    m_pReciveCountEdit->setText(tr("0"));
}

void SerialPortWidget::Autohave()
{

    if(m_pAutoRadioBut->isChecked()) {
        tmiems=m_pTimeEdit->text().toInt();
        m_pAutoTimer->start(tmiems);
        pSentButton->setEnabled(false);
    }else{
        m_pAutoTimer->stop();
        pSentButton->setEnabled(true);
    }
}

void SerialPortWidget::timeAuto()
{
   // if(haveopen)
    sendData();
        if(sentcont<18446744073709551610)
    sentcont++;
        else
            sentcont=0;
    m_pSentCountEdit->setText(QString::number(sentcont));
}

void SerialPortWidget::createImConnect(QString ObjName)
{
    disconnect(im->keyboard,SIGNAL(setvalue(QString)),m_pSendEdit,SLOT(setText(QString)));
     disconnect(im->keyboard,SIGNAL(setvalue(QString)),m_pTimeEdit,SLOT(setText(QString)));
    if(ObjName=="m_pSendEdit")
        connect(im->keyboard,SIGNAL(setvalue(QString)),m_pSendEdit,SLOT(setText(QString)));

    if(ObjName=="m_pTimeEdit")
        connect(im->keyboard,SIGNAL(setvalue(QString)),m_pTimeEdit,SLOT(setText(QString)));


}

void SerialPortWidget::sendData()
{
    QByteArray temp = m_pSendEdit->text().toUtf8();
    char *pBuff = temp.data();

    m_pMySerialPort->write(pBuff, temp.length());
}

void SerialPortWidget::openSerialPort()
{
    QByteArray name = m_pNameCombo->currentText().toUtf8();
    char *pName = const_cast<char*>(name.data());

    QByteArray parity = m_pParityCombo->currentText().toUtf8();
    char *pParity = parity.data();

//    char *pParity;
//    Transform::qstringToChar2(m_pParityCombo->currentText(), pParity);

    m_pMySerialPort->open(pName,
                         m_pBandrateCombo->currentText().toInt(),
                         m_pDataBitsCombo->currentText().toInt(),
                         pParity,
                         m_pStopBitsCombo->currentText().toInt());

    m_pReadThread->setWeiQianFunctions(m_pMySerialPort);
    m_pReadThread->start();
}

void SerialPortWidget::closeSerialPort()
{
    if(m_pReadThread->isRunning()) {
        m_pReadThread->stop();
        usleep(100000);
    }

    m_pMySerialPort->close();
}

void SerialPortWidget::cleanReceiveLabel()
{
    m_pReceiveEdit->setText("");
    m_pSendEdit->setText("");
    receivecont=0;
    m_pReciveCountEdit->setText("0");
    sentcont=0;
    m_pSentCountEdit->setText("0");
}

void SerialPortWidget::receiveData(const QString str)
{
    m_pReceiveEdit->append(str);
    if(receivecont<18446744073709551610)
    receivecont++;
    else
    receivecont=0;
  //  m_pReciveCountEdit->setText("");
  //  QString tmp =QString.toInt(receivecont);
    m_pReciveCountEdit->setText(QString::number(receivecont));
}

