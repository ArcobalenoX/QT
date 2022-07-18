#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDialog *dialog=new QDialog;
    dialog->setWindowTitle("enter your age");

    QSpinBox *spinbox=new QSpinBox;
    spinbox->setRange(0,130);
    spinbox->setValue(0);

    QSlider *slider=new QSlider(Qt::Horizontal);
    slider->setRange(0,130);

    //使spinbox 和 slider同步
    QObject::connect(spinbox,SIGNAL(valueChanged(int)),slider,SLOT(setValude(int)));
    QObject::connect(slider,SIGNAL(valueChanged(int)),spinbox,SLOT(setValue(int)));


    QGridLayout*layout=new QGridLayout;
    layout->addWidget(spinbox);
    layout->addWidget(slider);

    dialog->setLayout(layout);
    dialog->show();


    int arr[] = {1,8,4,6,1,2,5};
    int n=7;
    int t =0;
    for(int i=0;i<n;i++)
    {
            for(int j=i+1; j<n; j++)
            {
                if(arr[i]>arr[j])
                {
                 t= arr[i];
                 arr[i]=arr[j];
                 arr[j]=t;

                }

            }

    }
    for(int i=0;i<n;i++)
    {
        qDebug()<<arr[i];
    }





}

MainWindow::~MainWindow()
{
    delete ui;
}

