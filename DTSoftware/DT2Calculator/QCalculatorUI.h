#ifndef _QCALCULATORUI_H_
#define _QCALCULATORUI_H_

#include <QWidget>
#include <QLineEdit>
#include<QPushButton>

#include "ICalculator.h"

class QCalculatorUI : public QWidget
{
    Q_OBJECT    //SIGNAL SLOT 必需
private:
    QLineEdit* LineEdit ;
    QPushButton* buttons[20];
    ICalculator * calculator;

    QCalculatorUI();
    bool construct();   //二阶构造
private slots:
    void onButtonClicked();
public:
    static QCalculatorUI* NewInstance();
    void show();
    void setCalculator(ICalculator* cal);
    ICalculator* getCalculator();
    ~QCalculatorUI();
};

#endif // QCALCULATORUI_H
