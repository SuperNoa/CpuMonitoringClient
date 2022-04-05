#include <QCoreApplication>

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
    QCoreApplication a(argc, argv);

    TcpClient tcpClient;

    return a.exec();
}
