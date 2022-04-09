#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QTimer>

#include "WindowsHardwareInfo.h"

class QTcpSocket;

class TcpClient : public QObject
{

public:
    explicit TcpClient( QString serverAddress = "",
                        int serverPort = 0,
                        int monitoringRate = 0,
                        QObject *parent = nullptr);
    ~TcpClient();

    void sendCpuTemperature();

private:
    void initClient();
    void connectToServer();
    void stopConnection();

private:
    QTcpSocket*     m_tcpSocket;
    QHostAddress    m_hostAddress;
    quint16         m_port;

    QTimer*         m_temperatureReadingTimer;
    int             m_temperatureReadingRate;

    WindowsHardwareInfo m_windowsHardwareInfo;
};

#endif // TCPCLIENT_H
