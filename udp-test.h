#ifndef UDPTEST_H
#define UDPTEST_H

//#include <QUdpSocket>
#include "sender.h"
#include <QHostAddress>

class UDPTest :public QObject
{
    Q_OBJECT
public:
    UDPTest();
    ~UDPTest();

signals:
    void Start();

private slots:
    void Go();



private:
    //SockUDP *m_udpSock;
    bool SendData(QHostAddress address, quint16 port);
    clock_t m_timer;
    Sender *m_sender;
    QHostAddress m_address;
    quint16 m_port;


};


#endif // UDPTEST_H
