// mytcpserver.cpp

#include "tcpserver.h"
#include <QObject>
#include <glog/logging.h>

#include <QSettings>


MyTcpServer::MyTcpServer(QObject *parent) :
        QObject(parent)
{

    server = new QTcpServer(this);

    QSettings settings("settings/server.ini", QSettings::IniFormat);

    int puerto=settings.value("ConfiguracionRed/PuertoEscucha").toInt();

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),
            this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::LocalHost, puerto))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
        LOG(INFO) << "Server started! at port: " << server->serverPort();

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