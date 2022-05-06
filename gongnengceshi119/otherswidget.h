#ifndef OTHERSWIDGET_H
#define OTHERSWIDGET_H

#include <QWidget>
#include "InputMethod/inputmethod.h"
QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class OthersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OthersWidget(QWidget *parent = 0);
    InputMethod *im;
signals:

public slots:


private slots:
    void closeBackLight();
    void openBackLight();
    void setMac();
    void setBeep();
    void createImConnect(QString ObjName);

private:
    QLineEdit       *m_pBackLightEdit;
    QLineEdit       *m_pMacEdit;
    QPushButton     *m_pBeepBut;

};

#endif // OTHERSWIDGET_H
