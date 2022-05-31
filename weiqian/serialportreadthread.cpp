#include "serialportreadthread.h"
#include <QDebug>

SerialPortReadThread::SerialPortReadThread(QObject *parent) :
    QThread(parent),
    stopped(false)
{

}

void SerialPortReadThread::setWeiQianFunctions(SerialPort *serialPort)
{
    m_pSerialPort = serialPort;
}

void SerialPortReadThread::stop()
{
    stopped = true;
}

void SerialPortReadThread::run()
{
    int lenth=0;
    while(!stopped) {
        QString str ;
        char *pBuff = new char[64];
        int size = m_pSerialPort->read(pBuff, 64);
        if(size > 0) {
           // const QString str = QString(QLatin1String(pBuff));

            for(int i =0;i<size;i++) {
                QByteArray ba ;
                ba.append(pBuff[i]);
                if(ba==0)
                    str.append("00");
                 else
                    str.append(ba.toHex());
                if(i!=(size))
                str.append(" ");
            }

            emit receiveSignal(str);
        }
            delete pBuff;
    }
    stopped = false;
}
