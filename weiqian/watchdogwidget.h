#ifndef WATCHDOGWIDGET_H
#define WATCHDOGWIDGET_H

#include <QWidget>
#include "InputMethod/inputmethod.h"
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QRadioButton;
class QTimer;
QT_END_NAMESPACE

class WatchDogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WatchDogWidget(QWidget *parent = 0);
    InputMethod *im;

signals:

public slots:


private slots:
    void watchdogEnable();
    void watchdogDisable();
    void watchdogOpen();
    void watchdogClose();
    void watchdogSetTime();
    void watchdogGetTime();
    void watchdogFeedDog();
    void watchdogCountDown();
    void createImConnect(QString ObjName);

private:
    QLabel      *m_pGetTimeLabel;
    QLineEdit   *m_pSetTimeEdit;
    QLabel      *m_pCountDownLabel;

    QRadioButton    *m_pEnableRadioBut;
    QRadioButton    *m_pDisableRadioBut;
    QRadioButton    *m_pOpenRadioBut;
    QRadioButton    *m_pCloseRadioBut;

    QTimer      *m_pCountDownTimer;
    unsigned int m_count;
};

#endif // WATCHDOGWIDGET_H
