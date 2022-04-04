#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>

class QTcpSocket;

class TcpClient : public QObject
{

public:
    explicit TcpClient(QObject *parent = nullptr);

    void sendCpuTemperature();

private:
    void initClient();

private:
    QTcpSocket* m_tcpSocket;
    QDataStream m_in;
};

#endif // TCPCLIENT_H
