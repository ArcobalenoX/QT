#ifndef _QCALCULATOR_H_
#define _QCALCULATOR_H_

#include "QCalculatorUI.h"
#include "QCalculatorDec.h"

class QCalculator
{
protected:
    QCalculatorUI* m_ui;
    QCalculatorDec m_cal;

    QCalculator();
    bool construct();
public:
    static QCalculator* NewInstance();
    void show();
    ~QCalculator();
};

#endif // QCALCULATOR_H
