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
class Mobj :public QObject
{
    QString m_name;

public:
    Mobj(QString name)
    {
        m_name = name;
        qDebug()<<"create "<<m_name;
    }

    ~Mobj()
    {
        qDebug()<<"delete "<<m_name;
    }

};


void deltest()
{
    Mobj* obj1 = new Mobj("obj1");
    Mobj* obj2 = new Mobj("obj2");
    Mobj* obj3 = new Mobj("obj3");
    Mobj* obj4 = new Mobj("obj4");

    obj2->setParent(obj1);
    obj3->setParent(obj1);
    obj4->setParent(obj3);

    delete obj3;
    delete obj1;

}


void objecttest()
{
    QObject* p  = new QObject();
    QObject* c1 = new QObject();
    QObject* c2 = new QObject();

    c1->setParent(p);
    c2->setParent(p);

    qDebug()<<"c1 addr: "<<c1;
    qDebug()<<"c2 addr: "<<c2;

    const QObjectList list = p->children();
    for(int i=0;i<list.length();i++)
    {
        qDebug()<<list[i];
    }

    qDebug()<<"p addr: "<<p;
    qDebug()<<"c1 parent: "<<c1->parent();
    qDebug()<<"c2 parent: "<<c2->parent();

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
    qDebug("window x:%d y:%d width:%d height:%d\r\n",mw.x(),mw.y(),mw.width(),mw.height());

    QPushButton close(&mw);
    close.move(300,200);
    close.resize(40,20);
    close.setText("close");

    mw.show();

    QObject::connect(&close,SIGNAL(clicked()),&a,SLOT(quit()));


    QRect geo = mw.geometry();
    cout<<"geo "<<geo.x()<<" "<<geo.y()<<" "<<geo.width()<<" "<<geo.height()<<endl;
    QRect frame = mw.frameGeometry();
    qDebug("frame x:%d y:%d width:%d height:%d\r\n",frame.x(),frame.y(),frame.width(),frame.height());

    QWidget ww(NULL,Qt::SplashScreen);
    ww.setWindowTitle("widget");

    QDialog dw(&ww);
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


    //objecttest();
    //deltest();

    return a.exec();
}
