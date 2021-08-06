#ifndef _CALCULATORCORE_H_
#define _CALCULATORCORE_H_

#include <QString>
#include <QStack>
#include <QQueue>
#include <QDebug>
#include "ICalculator.h"

class QCalculatorDec : public ICalculator
{
protected:
    QString m_exp;
    QString m_result;

    bool isDigitOrDot(QChar c);
    bool isSymbol(QChar c);
    bool isSign(QChar c);
    bool isNumber(QString s);
    bool isOperator(QString s);
    bool isLeft(QString s);
    bool isRight(QString s);
    int priority(QString s);
    bool match(QQueue<QString>& exp);
    QString calculate(QQueue<QString>& exp);
    QString calculate(QString l, QString op, QString r);
    bool transform(QQueue<QString>& exp, QQueue<QString>& output);
    QQueue<QString> split(const QString& exp);
public:
    QCalculatorDec();
    ~QCalculatorDec();
    bool expression(const QString& exp);
    QString expression();
    QString result();
};

#endif
