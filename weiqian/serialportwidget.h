#ifndef SERIALPORTWIDGET_H
#define SERIALPORTWIDGET_H

#include <QWidget>
#include <QCheckBox>
#include "InputMethod/inputmethod.h"
QT_BEGIN_NAMESPACE
class QComboBox;
class QTextEdit;
class QLineEdit;
class SerialPort;
class SerialPortReadThread;
QT_END_NAMESPACE

class SerialPortWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SerialPortWidget(QWidget *parent = 0);
    InputMethod *im;
    unsigned int tmiems;
private slots:
    void sendData();
    void openSerialPort();
    void closeSerialPort();
    void cleanReceiveLabel();
    void receiveData(const QString str);
    void createImConnect(QString ObjName);
    void Autohave();
    void timeAuto();

private:
    QTextEdit		*m_pReceiveEdit;
    QLineEdit		*m_pSendEdit;
    QLineEdit		*m_pTimeEdit;
    QLineEdit		*m_pSentCountEdit;
    QLineEdit		*m_pReciveCountEdit;
    QTimer			*m_pReceiveTimer;
    QComboBox		*m_pBandrateCombo;
    QComboBox		*m_pDataBitsCombo;
    QComboBox		*m_pParityCombo;
    QComboBox		*m_pStopBitsCombo;
    QComboBox       *m_pNameCombo;
    SerialPort      *m_pMySerialPort;
    SerialPortReadThread    *m_pReadThread;
    QCheckBox *m_pAutoRadioBut;
    QString * timetmp;
    QTimer      *m_pAutoTimer;
    QPushButton		*pSentButton;
};

#endif // SERIALPORTWIDGET_H
