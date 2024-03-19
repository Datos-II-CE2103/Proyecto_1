#include "tcpserver.h"
#include <QObject>

#include <QSettings>

using namespace std;

MyTcpServer::MyTcpServer(QObject *parent) :
        QObject(parent)
{
    server = new QTcpServer(this);

    QSettings settings("../settings/server.ini", QSettings::Format::IniFormat);

    int puerto = settings.value("configuracionred/puertoescucha").toInt();

    qDebug() << "Puerto leído desde el archivo INI:" << puerto;

    connect(server, SIGNAL(newConnection()),
            this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::LocalHost, puerto))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}


void MyTcpServer::newConnection()
{
    // need to grab the socket
    QTcpSocket *socket = server->nextPendingConnection();

    socket->write("Hello client\r\n");
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();
}