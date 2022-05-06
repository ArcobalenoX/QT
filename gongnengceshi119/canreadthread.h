#ifndef CANREADTHREAD_H
#define CANREADTHREAD_H

#include <QThread>

class CanReadThread : public QThread
{
    Q_OBJECT
public:
    explicit CanReadThread(QObject *parent = 0);

    void stop();

signals:
    void receiveSignal(const int id, const QString data, const int lenght);

public slots:

protected:
    void run();
    unsigned char asciiToHex(unsigned char aHex);

private:
    volatile bool stopped;
};

#endif // CANREADTHREAD_H
