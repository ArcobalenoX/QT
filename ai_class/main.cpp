#include "widget.h"
#include <QApplication>
#include <QtMultimedia/QMediaPlayer>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

/*  QMediaPlayer *p= new QMediaPlayer;
    p->setMedia(QUrl::fromLocalFile("C:\\Users\\ZHS\\Downloads\\ok.mp3"));
    p->play();*/

    Widget w;
    w.show();

    return a.exec();
}
