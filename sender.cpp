#include "sender.h"
//#include <QUdpSocket>
#include <iostream>
#include <QCoreApplication>
#include <cstring>
#include <QDebug>

#include <sys/ioctl.h>
#include <linux/sockios.h>

#define MTU_SIZE 1400
#define DATA_SIZE 400000


Sender::Sender()
{
    m_timer=NULL;
    m_frameCount=0;
    m_frameRate=10;
    m_MTUSize=MTU_SIZE;
    m_data=NULL;
    m_socket=NULL;
    m_dataSize=DATA_SIZE;
    m_port=0;
    m_fd=0;

}

Sender::~Sender()
{
    if(m_timer) {m_timer->stop(); delete m_timer; m_timer=NULL;}
    if(m_socket) {m_socket->close(); delete m_socket; m_socket=NULL;}
    if(m_data) {free(m_data); m_data=NULL;}


}

bool Sender::Send(QHostAddress address, quint16 port)
{
    m_data=GenData(DATA_SIZE);

    if(m_data==NULL)
    {
        qDebug() << "Failed to generate data";
        return false;
    }

    m_socket=new QUdpSocket();
    m_address=address;
    m_port=port;
    m_socket->connectToHost(address, port);
    m_fd=m_socket->socketDescriptor();
    m_socket->setSocketOption(QAbstractSocket::SendBufferSizeSocketOption, QVariant(0x600000));
    if(!SendFrame())
    {
        qDebug() << "Failed sending initial frame";
        return false;
    }
    m_frameCount++;
    m_timer=new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(StreamFrame()));
    m_timer->start(1000/m_frameRate);
    return true;
}

char* Sender::GenData(qint32 size)
{
    char *data=NULL;

    qDebug() << "Generating Data...";

    uchar value;
    data=(char*)malloc(size);
    if(!data) return NULL;

    for(qint32 i=0; i< size; i++)
    {
        value=rand() % (quint8)0xFF;
        memcpy(data+i, &value, 1);
    }
    qDebug() << "Done generating.";

    return data;
}

void Sender::StreamFrame()
{
    if(!SendFrame())
    {
        qDebug() << "Failed sending frame" << m_frameCount;
        m_timer->stop();
        delete m_timer;
        m_timer=NULL;
        exit(1);
    }
    m_frameCount++;
}

bool Sender::SendFrame()
{
    if(m_frameCount % 10 == 0) qDebug() << "Frame:" << m_frameCount;
    qint64 bytesWritten;
    qint64 totalBytesWritten;
    qint64 totalUDPBytesWritten;
    qint32 dgCount;
    qint32 offset;
    char datagram[MTU_SIZE];
    qint64 bytesInQtBuffer, bytesInSysBuffer;

    totalBytesWritten=0;
    bytesWritten=0;
    dgCount=0;
    offset=0;
    totalUDPBytesWritten=0;
    bytesInQtBuffer=0;
    bytesInSysBuffer=0;

    while(totalBytesWritten < m_dataSize)
    {
        memcpy(datagram, m_data+totalBytesWritten, std::min((quint32)(m_MTUSize), (quint32)(m_dataSize-totalBytesWritten)));
        totalBytesWritten+=std::min((quint32)(m_MTUSize), (quint32)(m_dataSize-totalBytesWritten));
        if(m_dataSize==totalBytesWritten)
        {
            //bytesWritten=m_socket->writeDatagram(datagram, m_dataSize-offset, m_address, m_port);
            bytesWritten=m_socket->write(datagram, m_dataSize-offset);
            if(bytesWritten >0) totalUDPBytesWritten+=bytesWritten;
            if(bytesWritten!=m_dataSize-offset)
            {
                if(bytesWritten==-1)
                {
                    qDebug() << "final write failed with Error: " << m_socket->errorString();
                }
                else
                {
                    qDebug() << "only sent " << bytesWritten << "/" << m_dataSize-offset << " bytes";
                }
                return false;
            }
            dgCount++;
            break;
        }


        //bytesWritten=m_socket->writeDatagram(datagram, m_MTUSize, m_address, m_port);
        bytesWritten=m_socket->write(datagram, m_MTUSize);


        if(bytesWritten >0)
        {
            totalUDPBytesWritten+=bytesWritten;
        }

        if(bytesWritten!=m_MTUSize)
        {
            if(bytesWritten==-1)
            {
                qDebug() << "write failed with Error:" << m_socket->errorString()
                         << "after" << dgCount << "datagrams.";
            }
            else
            {
                bytesInQtBuffer=m_socket->bytesToWrite();
                ioctl(m_fd, SIOCOUTQ, &bytesInSysBuffer);
                qDebug() << "only sent " << bytesWritten << "/" << m_MTUSize << "bytes after" << dgCount << "datagrams.";
                qDebug() << bytesInQtBuffer << "bytes in qt buffer.";
                qDebug() << bytesInSysBuffer << "/" << m_socket->socketOption(QAbstractSocket::SendBufferSizeSocketOption)<< "bytes in system buffer.";

            }
            return false;
        }
        dgCount++;
        offset=totalBytesWritten;
    }
    return true;
}
