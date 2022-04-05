#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QTimer>

class QTcpSocket;

class TcpClient : public QObject
{

public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();

    void sendCpuTemperature();

private:
    void initClient();
    void connectToServer();
    void stopConnection();

private:
    QTcpSocket*     m_tcpSocket;
    QHostAddress    m_hostAddress;
    quint16         m_port { 0 };

    QTimer*         m_temperatureReadingTimer;
    int             m_temperatureReadingRate { 1000 };
};

#endif // TCPCLIENT_H
