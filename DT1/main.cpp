#include "mainwindow.h"

#include <QApplication>
#include <QMainWindow>
#include <QtWidgets>
#include <QDebug>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QRect>


#include <iostream>
using namespace std;


void example_1()
{
    QString s = "and";
    s.append(" ");
    s.append("Qt");
    s.append(" ");
    s.prepend("C++ ");
    qDebug()<<s;
    s.replace("and","&");
    qDebug()<<s;
}

void example_2()
{
    QString s = "";
    int index = 0;
    s = QString:: asprintf("%d. I'm %s,aha?",1,"gg");
    qDebug()<<s;    //"1. I'm gg,aha?"

    index = s.indexOf(",");
    s = s.mid(0,index);
    qDebug()<<s;    //"1. I'm gg"

    index = s.indexOf(".");
    s = s.mid(index+1,s.length());
    s = s.trimmed();
    qDebug()<<s;    //"I'm gg"


    index = s.indexOf(" ");
    s = s.mid(index+1,s.length());
    s = s.trimmed();
    qDebug()<<s;    //"gg"



}

void example_3(QString* arr,int len)
{
    for(int i=0; i<len; i++)
    {
        for(int j=i+1; j<len; j++)
        {
            if(arr[i]>arr[j])
            {
                QString tmp =arr[i];
                arr[i]=arr[j];
                arr[j]=tmp;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug()<<"example_1";
    example_1();
    qDebug()<<"example_2";
    example_2();

    qDebug()<<"example_3";
    QString company[5] =
    {
        QString("Oracle"),
        QString("Borland"),
        QString("Microsoft"),
        QString("IBM"),
        QString("DT")
    };
    example_3(company,5);
    for(int i=0; i<5; i++)
    {
        qDebug()<<company[i];
    }

    MainWindow mw;
    mw.setWindowTitle("MainWindow");
    mw.resize(400,300);

    QLineEdit* LineEdit ;
    QPushButton* buttons[20];
    const char* btnTexts[20]=
    {
        "7","8","9","(",")",
        "4","5","6","+","-",
        "1","2","3","*","/",
        "0",".","←","=","C",
    };
    LineEdit = new QLineEdit(&mw);
    if(LineEdit !=NULL)
    {
        LineEdit->move(10,10);
        LineEdit->resize(240,30);
        LineEdit->setReadOnly(true);
    }

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<5;j++)
        {
            buttons[i*5+j] = new QPushButton(&mw);
            if( buttons[i*5+j] != NULL)
            {
                buttons[i*5+j]->resize(40,40);
                buttons[i*5+j]->move(10+(10+40)*j,50+(10+40)*i);
                buttons[i*5+j]->setText(btnTexts[i*5+j]);
            }
        }
    }

    QPushButton close(&mw);
    close.move(300,200);
    close.resize(40,20);
    close.setText("close");

    mw.show();

    QObject::connect(&close,SIGNAL(clicked()),&a,SLOT(quit()));

    qDebug("window x:%d y:%d width:%d height:%d\r\n",mw.x(),mw.y(),mw.width(),mw.height());

    QRect geo = mw.geometry();
    cout<<"geo "<<geo.x()<<" "<<geo.y()<<" "<<geo.width()<<" "<<geo.height()<<endl;

    QRect frame = mw.frameGeometry();
    qDebug("frame x:%d y:%d width:%d height:%d\r\n",frame.x(),frame.y(),frame.width(),frame.height());


    /*
    QWidget ww(NULL,Qt::SplashScreen);
    ww.setWindowTitle("widget");

    QDialog dw;
    dw.setWindowTitle("dialog");

    QPushButton b(&ww);
    b.move(200,100);
    b.resize(100,50);
    b.setText("hello");

    QLabel l(&dw);
    l.move(300,100);
    l.setText("my label");
    ww.show();
    dw.show();
    ww.hide();
    dw.hide();
    */

    return a.exec();
}
