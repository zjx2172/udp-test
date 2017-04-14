#include "udp-test.h"
#include <QCoreApplication>
#include <QDebug>

UDPTest::UDPTest()
{
    srand(time(NULL));
    m_address.setAddress(QString("192.168.5.204"));
    m_port=1848;
    m_sender=NULL;
    connect(this, SIGNAL(Start()), this, SLOT(Go()));
    qDebug() << "Running...";
    emit Start();
}

UDPTest::~UDPTest()
{
//    if(m_udpSock) delete m_udpSock;
    if(m_sender) delete m_sender;
}


bool UDPTest::SendData(QHostAddress address, quint16 port)
{
    m_sender=new Sender();
    if(!m_sender->Send(address, port))
    {
        qDebug() << "m_sender->Send() failed. ";
//        app=QCoreApplication::instance();
//        app->exit(1);
        exit(1);
        return false;
    }
    return true;
}

void UDPTest::Go()
{
    if(!SendData(m_address, m_port))
    {
        qDebug() << "SendData() failed. ";

        return;
    }
    return;
}

/*void UDPTest::Start()
{
    qDebug() << "Starting Send.";
    return;
}*/


