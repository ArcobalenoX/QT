#include <QApplication>
#include "weiqiantools.h"
#include <QTextCodec>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QFont font;
    font.setPointSize(10);
    font.setFamily("BENMO Jingyuan Regular");
    font.setBold(false);
    a.setFont(font);

    WeiqianTools w;
    w.showMaximized();
    return a.exec();
}
