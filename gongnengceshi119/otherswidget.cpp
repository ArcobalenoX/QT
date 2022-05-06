#include "otherswidget.h"

#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QGroupBox>

#include "weiqianfunctions.h"
#include <stdio.h>
#include <unistd.h>

OthersWidget::OthersWidget(QWidget *parent) : QWidget(parent)
{
    im = new InputMethod;
    QLabel          *pBackLightLabel    = new QLabel;
    QLabel          *pMacLabel          = new QLabel;

    QPushButton     *pBackLightBut      = new QPushButton;
    QPushButton     *pMacBut            = new QPushButton;

                    m_pBackLightEdit    = new QLineEdit;
                    m_pMacEdit          = new QLineEdit;
                    m_pMacEdit->installEventFilter(im);
                    m_pMacEdit->setObjectName("m_pMacEdit");
                    m_pBackLightEdit->installEventFilter(im);
                    m_pBackLightEdit->setObjectName("m_pBackLightEdit");
                    connect(im,SIGNAL(setImObjName(QString)),this,SLOT(createImConnect(QString)));//connect the input method

   m_pMacEdit          ->setText(tr("98:13:a3:0f:22:01"));

system("ifconfig eth0 | grep HWaddr > /tmp/mactmp");

QFile file1("/tmp/mactmp");
file1.open(QFile::ReadOnly);
QString setpath=file1.readAll();
file1.close();
QString mactmp;
int began=0;
for(int i=0;i<setpath.length();i++)
{
 if(setpath[i]=='H' && setpath[i+1]=='W' && setpath[i+2]=='a'){
     began=1;
     i+=6;
 }
 if(began)
     if(setpath[i]!=' ' && setpath[i]!='\n')
     mactmp.append(setpath[i]);

}
   m_pMacEdit          ->setText(mactmp);
//  qDebug()<<m_pMacEdit->text().length();
//  qDebug()<<mactmp;
//  qDebug()<<m_pMacEdit->text();

                    m_pBeepBut          = new QPushButton;

    QGroupBox       *pBackLightGroup    = new QGroupBox;
    QGroupBox       *pMacGroup          = new QGroupBox;
    QGroupBox       *pBeepGroup         = new QGroupBox;

    QHBoxLayout     *pBackLightLayout   = new QHBoxLayout;
    QHBoxLayout     *pMacLayout         = new QHBoxLayout;
    QHBoxLayout     *pBeepLayout        = new QHBoxLayout;

    pBackLightLabel     ->setText(tr("关闭背光时间："));
    pBackLightBut       ->setText(tr("关闭背光"));
    m_pBackLightEdit    ->setText(tr("5"));
    pMacLabel           ->setText(tr("设置MAC地址："));
    pMacBut             ->setText(tr("设置MAC"));
    m_pBeepBut          ->setText(tr("蜂鸣器测试"));

    pBackLightGroup     ->setTitle(tr("背光测试"));
    pMacGroup           ->setTitle(tr("MAC测试"));
    pBeepGroup          ->setTitle(tr("蜂鸣器测试"));


    pBackLightLabel     ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBackLightBut       ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pBackLightEdit	->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pMacLabel			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pMacBut             ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pMacEdit			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pBeepBut          ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBackLightGroup     ->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pMacGroup			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pBeepGroup			->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    pBackLightLayout    ->addWidget(pBackLightLabel);
    pBackLightLayout    ->addWidget(m_pBackLightEdit);
    pBackLightLayout    ->addWidget(pBackLightBut);

    pMacLayout          ->addWidget(pMacLabel);
    pMacLayout          ->addWidget(m_pMacEdit);
    pMacLayout          ->addWidget(pMacBut);

    pBeepLayout         ->addWidget(m_pBeepBut);

    pBackLightGroup ->setLayout(pBackLightLayout);
    pMacGroup       ->setLayout(pMacLayout);
    pBeepGroup      ->setLayout(pBeepLayout);

    QGridLayout *pMainLayout = new QGridLayout;
    pMainLayout->addWidget(pBackLightGroup,     0,0,1,1);
    pMainLayout->addWidget(pMacGroup,           1,0,1,1);
    pMainLayout->addWidget(pBeepGroup,          2,0,1,1);

    this->setLayout(pMainLayout);

    connect(pBackLightBut,      SIGNAL(clicked()),      this,       SLOT(closeBackLight()));
    connect(pMacBut,            SIGNAL(clicked()),      this,       SLOT(setMac()));
    connect(m_pBeepBut,         SIGNAL(clicked()),      this,       SLOT(setBeep()));

}

void OthersWidget::createImConnect(QString ObjName)
{
    disconnect(im->keyboard,SIGNAL(setvalue(QString)),m_pMacEdit,SLOT(setText(QString)));
    disconnect(im->keyboard,SIGNAL(setvalue(QString)),m_pBackLightEdit,SLOT(setText(QString)));
    if(ObjName=="m_pMacEdit")
        connect(im->keyboard,SIGNAL(setvalue(QString)),m_pMacEdit,SLOT(setText(QString)));
    if(ObjName=="m_pBackLightEdit")
        connect(im->keyboard,SIGNAL(setvalue(QString)),m_pBackLightEdit,SLOT(setText(QString)));
}

void OthersWidget::closeBackLight()
{
    WeiqianFunctions::SetBackLight(false);
    QTimer::singleShot(m_pBackLightEdit->text().toInt()*1000, this, SLOT(openBackLight()));
}

void OthersWidget::openBackLight()
{
    WeiqianFunctions::SetBackLight(true);
}

void OthersWidget::setMac()
{
    //qDebug("efgefg");
 //     qDebug()<<m_pMacEdit->text();
  //     qDebug()<<m_pMacEdit->text().length();
     if( (m_pMacEdit->text().length()!=17)) {
     QDialog * setdialog = new QDialog;
     QLabel * settmp = new QLabel;
     settmp->setText(tr("MAC地址格式不对!"));
     settmp->setStyleSheet("color:red;");
     QTimer*timer = new QTimer(setdialog);
     timer->start(800);
     connect(timer,SIGNAL(timeout()),setdialog,SLOT(close()));
     QGridLayout *tmplayout = new QGridLayout;
     tmplayout->addWidget(settmp,0,0,1,1);
     setdialog->setLayout(tmplayout);
     setdialog->setWindowFlags(Qt::FramelessWindowHint);
     setdialog->exec();
}else if( (m_pMacEdit->text().length()==17)){

//         char *S;
//          system("ifconfig eth0 down");
//          QString tmp="ifconfig eth0 hw ether ";
//          tmp.append(m_pMacEdit->text().toUtf8());
//          S = (tmp.toAscii()).data();
//          system(S);
//          usleep(1000);
//          system("ifconfig eth0 up");

//          QFile::remove("/etc/zjdm");
//          QFile file1("/etc/zjdm");
//          file1.open(QFile::WriteOnly);
//          QTextStream s(&file1);
//          s<<m_pMacEdit->text();
//          file1.close();


    QByteArray temp = m_pMacEdit->text().toUtf8();
    const char *pMac = temp.data();

    WeiqianFunctions::SetMacAddress(pMac);
    usleep(1000);
    qDebug("rrrr");
//      QFile file1("/tmp/zjdm");
//      file1.open(QFile::WriteOnly);
//      QTextStream s(&file1);
//      s<<m_pMacEdit->text();
//      file1.close();
//      system("cat /etc/macaddr > /tmp/zjdm");
      system("sync");

     }
}

void OthersWidget::setBeep()
{
    WeiqianFunctions::Beep();
}



