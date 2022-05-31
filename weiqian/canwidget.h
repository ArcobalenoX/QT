#ifndef CANWIDGET_H
#define CANWIDGET_H

#include <QWidget>
#include "InputMethod/inputmethod.h"
#include <QCheckBox>

QT_BEGIN_NAMESPACE
class QComboBox;
class QTextEdit;
class QLineEdit;
class QTimer;
class CanReadThread;
QT_END_NAMESPACE

class CanWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CanWidget(QWidget *parent = 0);
    InputMethod *im;
    unsigned int tmiems;

signals:

public slots:

private slots:
    void openCan();
    void closeCan();
    void clear();
    void send();
    void receiveData(const int id, const QString data, const int lenght);
    void createImConnect(QString ObjName);
    void Autohave();
    void timeAuto();


private:
    QTextEdit   *m_pReceiveEdit;
    QComboBox   *m_pBaudCombo;
    QLineEdit   *m_pSendEdit;
    QLineEdit   *m_pCanIdEdit;
    CanReadThread  *m_pThread;
     QPushButton     *pSentBut;
     QCheckBox *m_pAutoRadioBut;
     QString * timetmp;
     QTimer      *m_pAutoTimer;
     QLineEdit		*m_pTimeEdit;
     QLineEdit		*m_pSentCountEdit;
     QLineEdit		*m_pReciveCountEdit;
     QTimer			*m_pReceiveTimer;
};

#endif // CANWIDGET_H
