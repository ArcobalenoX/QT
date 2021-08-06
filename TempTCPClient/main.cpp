#include "mainwindow.h"
#include <QApplication>

#include<iostream>
#include<cstdlib>
#include<unistd.h>
#include<time.h>

#include<QTcpSocket>
#include<QHostAddress>
#include<QDebug>
#include<QThread>

using namespace std;
short gTemp;
void makeTemp()
{
    srand(clock());
    short temp = 0;
    char high = 0;
    char low = 0;
    while(1)
    {
        high = 34+rand()%10;
        low = rand()%100;
        temp = (high<<8) + low;
        gTemp = temp;
        sleep(1);
        //cout <<hex<<gTemp<<endl;
        cout<<(high+low/100.0)<<endl;
    }

}

int main(int argc, char *argv[])
{
    //多线程
    QThread:: create(makeTemp)->start();
    //TCP
    QTcpSocket *client = new QTcpSocket();
    QHostAddress address("127.0.0.1");
    client->connectToHost(address,8888);
    if(client->waitForConnected())
    {
        //对方的地址端口
        qDebug()<<client->peerAddress();
        qDebug()<<client->peerPort();
        //自己的地址端口
        qDebug()<<client->localAddress(); qDebug()<<client->localPort();
    }

    while(1)
    {
        sleep(rand()%5);
        qDebug()<<client->write((char*)&gTemp,sizeof (gTemp));
        client->flush();
    }
    return 0;
}
