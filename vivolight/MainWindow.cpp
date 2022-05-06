#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    this->setGeometry(600,600,400,300);

    label1 = new QLabel(this);
    label1->resize(30,20);
    label1->setText("导管");
    label1->move(30,50);

    label2 = new QLabel(this);
    label2->resize(30,20);
    label2->setText("手柄");
    label2->move(60,50);

    label3 = new QLabel("系统",this);
    label3->resize(30,20);
    label3->move(90,50);

    wid = new QWidget(this);
    wid->setGeometry(100,100,300,200);

    vboxlayout = new QVBoxLayout(this);
    vboxlayout->addWidget(label1);
    vboxlayout->addWidget(label2);
    vboxlayout->addWidget(label3);
    vboxlayout->setSpacing(50);
    wid->setLayout(vboxlayout);


//   this->setCentralWidget(wid);


}

MainWindow::~MainWindow()
{
    delete label1;
    delete label2;
    delete label3;
    delete vboxlayout;


}

