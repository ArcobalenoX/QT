#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>
#include <QTemporaryFile>
#include <QTextStream>

void fileinfo(QString path)
{
    QFile file(path);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    file.write("abcdefg",8);
    QByteArray ba;
    ba = file.read(10);
    qDebug()<<QString(ba);
    QTextStream in(&file);
    in<<"123456789";
    QFileInfo info(file);
    qDebug()<<info.absolutePath();
    //qDebug()<<info.created();
    qDebug()<<info.lastModified();
    qDebug()<<info.path();
    file.close();
    QTemporaryFile tf;
    tf.open();
    QFileInfo tfinfo(tf);
    qDebug()<<tfinfo.fileName();
    qDebug()<<tfinfo.absolutePath();
    tf.close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow* w = MainWindow::newInstance();
    int ret = -1;

    //fileinfo("note.txt");

    if( w != NULL )
    {
        w->show();
        ret = a.exec();
    }

    delete w;
    return ret;
}
