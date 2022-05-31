#include "canreadthread.h"
#include "weiqianfunctions.h"
#include <QDebug>
CanReadThread::CanReadThread(QObject *parent) :
    QThread(parent),
    stopped(false)
{

}

void CanReadThread::stop()
{
    stopped = true;
}

unsigned char CanReadThread::asciiToHex(unsigned char aHex)
{
    if( (aHex>=0) && (aHex<=9) ){
        aHex+=0x30;

    }
    else if( (aHex>=10) && (aHex<=15) )
        aHex+=0x37;
    else {
        aHex=0xFF;
    }
    return aHex;
}

void CanReadThread::run()
{
    while(!stopped) {

        int lenght;
        char buff[16];
        QString str;
        int id = WeiqianFunctions::CanRead(&lenght, buff);
        if((id != 0) && (lenght > 0)) {

            const int len = lenght;
            const int i = id;
            for(int i =0;i<8;i++) {
                QByteArray ba ;
                ba.append(buff[i]);

                if(ba==0)
                    str.append("00");
                 else
                    str.append(ba.toHex());
                if(i!=7)
                str.append(" ");
            }

            emit receiveSignal(i, str, len);

        }
    }
    stopped = false;
}
