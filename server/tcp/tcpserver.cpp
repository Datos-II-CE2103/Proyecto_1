#include "tcpserver.h"
#include <QObject>
#include <glog/logging.h>
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/stringbuffer.h"
#include <iostream>
#include <QSettings>

using namespace std;
using namespace rapidjson;


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

    const char* json = "{\"status\":\"ok\",\"stars\":10}";
    rapidjson::Document d;
    d.Parse(json);

    // 2. Modify it by DOM.
    rapidjson::Value& s = d["stars"];
    s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    rapidjson::StringBuffer buffer;
    Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    socket->write(buffer.GetString());
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();
}