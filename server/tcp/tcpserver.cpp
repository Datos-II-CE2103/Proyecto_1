#include "tcpserver.h"
#include <QObject>
#include <glog/logging.h>

#include <QSettings>

using namespace std;

MyTcpServer::MyTcpServer(QObject *parent) :
        QObject(parent)
{
    server = new QTcpServer(this);

    QSettings settings("../settings/server.ini", QSettings::Format::IniFormat);

    int puerto = settings.value("configuracionred/puertoescucha").toInt();

    qDebug() << "Puerto leído desde el archivo INI (ss):" << puerto;

    connect(server, SIGNAL(newConnection()),
            this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::LocalHost, puerto))
    {
        qDebug() << "Server could not start";
        LOG(ERROR) << "Server could not started at port: " << server->serverPort();
    }
    else
    {
        qDebug() << "Server started!";
        LOG(INFO) << "Server started! at port: " << server->serverPort();
    }
}


void MyTcpServer::newConnection()
{
               this->nuevaConex();
    //need to grab the socket
    QTcpSocket *socket = server->nextPendingConnection();
    LOG(INFO) << "Nueva conxeion entrante al servidor desde un cliente";
    socket->write("{\n"
                  "\"status\": \"ok\",\n"
                  "\"age\": 43,\n"
                  "\"phones\": [\n"
                  "\"+44 1234567\",\n"
                  "\"+44 2345678\"\n"
                  "]\n"
                  "}");
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();
}