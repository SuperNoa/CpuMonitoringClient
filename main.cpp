#include <QCoreApplication>
#include <QCommandLineParser>

#include "TcpClient.h"

//  You are requested to create a small client-server application for monitoring the CPU
//  temperature of the client PC over a network.
//  The client reads the CPU temperature of the PC and sends it via network to the monitoring server every n seconds.
//  The rate of the temperature reading must be configurable by the user at launch time.
//  The server, once the client is connected, graphs the CPU temperature of the client PC on a plot.
//  If the temperature rises above a certain threshold that can be set by the user, then an alarm is raised.
//  You are required to use the Qt framework and C++ to develop the server.
//  You are free to choose how to develop the client (for example a Python solution might be easier to implement than a C++ based one)

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("CPU load monitor client");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("A client application to monitor the CPU load");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("rate",    QCoreApplication::translate("main", "Monitoring rate in ms."));
    parser.addPositionalArgument("address", QCoreApplication::translate("main", "Server address."));
    parser.addPositionalArgument("port",    QCoreApplication::translate("main", "Server port."));

    QCommandLineOption monitoringRateOption("rate",     QCoreApplication::translate("main", "Monitoring rate in ms."), "1000");
    parser.addOption(monitoringRateOption);

    QCommandLineOption serverAddressOption("address",   QCoreApplication::translate("main", "Server address."), "127.0.01");
    parser.addOption(serverAddressOption);

    QCommandLineOption serverPortOption("port",         QCoreApplication::translate("main", "Server port."), "5000");
    parser.addOption(serverPortOption);

    parser.process(app);

    bool monitoringRateSet  = parser.isSet(monitoringRateOption);
    bool serverAddressSet   = parser.isSet(serverAddressOption);
    bool serverPortSet      = parser.isSet(serverPortOption);

    int monitoringRate      { 250 };
    QString serverAddress   { "127.0.0.1" };
    int serverPort          { 5000 };

    monitoringRate          = parser.value(monitoringRateOption).toInt();
    serverAddress           = parser.value(serverAddressOption);
    serverPort              = parser.value(serverPortOption).toInt();

    qDebug() << ( monitoringRateSet ?
                      QString("The client will send data to the server each %1").arg(monitoringRate).append(" ms") :
                      QString("Monitoring rate not specified, the client will send data to the server each ").arg(monitoringRate).append(" ms") );

    qDebug() << ( serverAddressSet ?
                      QString("The client will connect to the server at the address ").append(serverAddress) :
                      QString("Server address not specified, the client will use ").append(serverAddress) );

    qDebug() << ( serverPortSet ?
                      QString("The client will use the port %1 ").arg(serverPort) :
                      QString("Server port not specified, the client will use %1").arg(serverPort) );

    TcpClient tcpClient { serverAddress, serverPort, monitoringRate };

    return app.exec();
}
