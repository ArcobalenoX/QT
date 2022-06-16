#include <QApplication>
#include<QDebug>
#include<QDialog>

#include "dialog.h"
#include "widget.h"

void dialogtest()
{
    Dialog dlg;
    dlg.show();

    int r = dlg.exec();

    if( r == QDialog::Accepted )
    {
        qDebug() << "Accepted";
    }
    else if( r == QDialog::Rejected )
    {
        qDebug() << "Rejected";
    }
    else
    {
        qDebug() << r;
    }

}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //dialogtest();


    Widget w;
    w.setWindowTitle("Dialog&Layout");
    w.show();

    return a.exec();
}


