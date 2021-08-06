#include "mainwindow.h"
#include <QApplication>
#include <QDialog>
#include <QSpinBox>
#include <QSlider>
#include <QHBoxLayout>

#include "finddialog.h"
#include "gotocelldialog.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
   // w.show();


    QWidget *window=new QWidget;
    window->setWindowTitle("enter your age");

    QSpinBox *spinbox=new QSpinBox;
    spinbox->setRange(0,130);
    spinbox->setValue(0);

    QSlider *slider=new QSlider(Qt::Horizontal);
    slider->setRange(0,130);

    //使spinbox 和 slider同步
    QObject::connect(spinbox,SIGNAL(valueChanged(int)),slider,SLOT(setValude(int)));
    QObject::connect(slider,SIGNAL(valueChanged(int)),spinbox,SLOT(setValue(int)));


    //QHBoxLayout;QVBoxLayout;QGridLayout
    QGridLayout*layout=new QGridLayout;
    layout->addWidget(spinbox);
    layout->addWidget(slider);

    window->setLayout(layout);
    window->show();

    FindDialog *dialog = new FindDialog;
    dialog->show();



    return app.exec();
}
