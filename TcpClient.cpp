#include "TcpClient.h"

#include <QTcpSocket>
#include <QHostInfo>
#include <QNetworkInterface>

TcpClient::TcpClient(QObject *parent) :
    QObject(parent),
    m_tcpSocket(new QTcpSocket(this))
{
    initClient();

    QDataStream in;

    in.setDevice(m_tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(m_tcpSocket, &QAbstractSocket::connected, []
        {
            qDebug() << "Connected";
        }
    );

    //connect(m_tcpSocket, &QIODevice::readyRead, this, &Client::readFortune);

    connect(m_tcpSocket, &QAbstractSocket::errorOccurred, []
        {

        }
    );
}

void TcpClient::sendCpuTemperature()
{
    m_tcpSocket->abort();

    m_tcpSocket->connectToHost(QHostAddress::LocalHost, 5000);
}

void TcpClient::initClient()
{
    // find out name of this machine
    QString name = QHostInfo::localHostName();

    if (!name.isEmpty())
    {
        QString domain = QHostInfo::localDomainName();

        // find out IP addresses of this machine
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

        // add non-localhost addresses
        for (int i = 0; i < ipAddressesList.size(); ++i)
        {
            if ( !ipAddressesList.at(i).isLoopback() )
            {
                QString address = ipAddressesList.at(i).toString();

                qDebug() << "non-localhost " << address;
            }
        }

        // add localhost addresses
        for (int i = 0; i < ipAddressesList.size(); ++i)
        {
            if (ipAddressesList.at(i).isLoopback())
            {
                QString address = ipAddressesList.at(i).toString();
                qDebug() << "localhost " << address;
            }
        }
    }
}
