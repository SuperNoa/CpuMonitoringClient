#include "TcpClient.h"

#include <QTcpSocket>
#include <QNetworkInterface>

TcpClient::TcpClient(QObject *parent) :
    QObject(parent)
{
    initClient();

    connectToServer();
}

TcpClient::~TcpClient()
{
    m_temperatureReadingTimer->stop();

    m_tcpSocket->disconnectFromHost();
}

void TcpClient::initClient()
{
    m_tcpSocket                 = new QTcpSocket(this);
    m_temperatureReadingTimer   = new QTimer(this);

    m_hostAddress   = QHostAddress::LocalHost;
    m_port          = 5000;

    connect(m_temperatureReadingTimer, &QTimer::timeout, this, QOverload<>::of(&TcpClient::sendCpuTemperature));
}

void TcpClient::connectToServer()
{
    m_tcpSocket->abort();

    m_tcpSocket->connectToHost(m_hostAddress, m_port);

    connect(m_tcpSocket, &QAbstractSocket::connected, [this]
        {
            qDebug() << tr("Client connected at %1 on the port %2.").arg( m_hostAddress.toString() ).arg( QString::number(m_port) );

            m_temperatureReadingTimer->start( m_temperatureReadingRate );
        }
    );

    connect(m_tcpSocket, &QAbstractSocket::errorOccurred, [this] (QAbstractSocket::SocketError socketError)
        {
             qDebug() << tr("The following error occurred: %1.").arg( m_tcpSocket->errorString() );
        }
    );
}

void TcpClient::stopConnection()
{
    m_temperatureReadingTimer->stop();

    connect(m_temperatureReadingTimer, &QTimer::timeout, [this]
        {
            m_tcpSocket->disconnectFromHost();
        }
    );
}

void TcpClient::sendCpuTemperature()
{
    // Encode the temperature into a QByteArray using QDataStream
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    // Set the protocol version of QDataStream to Version 20 (Qt 6.0)
    out.setVersion(QDataStream::Qt_6_2);

    QString temperature { "Temperatura corrente: 38°" };

    // write it to the connecting socket
    out << temperature;

    m_tcpSocket->write(block);
}
