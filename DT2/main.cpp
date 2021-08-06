#include <QApplication>
#include <QLineEdit>
#include<QLabel>
#include<QPushButton>

#include "QCalculator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int ret = -1;

    QCalculatorUI* calcul = QCalculatorUI::NewInstance();
    if(calcul != NULL)
    {
        calcul->show();
        ret = a.exec();
        delete calcul;
    }

    return ret;
}
