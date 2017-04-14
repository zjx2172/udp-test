#ifndef SENDER_H
#define SENDER_H

#include <QHostAddress>
#include <QUdpSocket>
#include <QTimer>

class Sender : public QObject
{
    Q_OBJECT

public:
    Sender();
    ~Sender();

    bool Send(QHostAddress address, quint16 port);
    QUdpSocket *m_socket;

private slots:
    void StreamFrame();

private:
//    char m_address[INET_ADDRSTRLEN];
//    quint16 m_port;
    QTimer *m_timer;
    quint64 m_frameCount;
    quint8  m_frameRate;
    quint16 m_MTUSize;
    char *m_data;
    qint32 m_dataSize;

    char* GenData(qint32 size);
    bool SendFrame();

    quint16 m_port;
    QHostAddress m_address;
    qint64 m_fd;
};

#endif // SENDER_H
