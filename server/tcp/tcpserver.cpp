#include "tcpserver.h"
#include <QObject>
#include <glog/logging.h>
#include "../include/json.hpp"
#include <iostream>
#include <QSettings>

using namespace std;
using namespace rapidjson;


MyTcpServer::MyTcpServer(QObject *parent) :
        QObject(parent)
{
    comunitario= false;
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
    if(comunitario){
        const QByteArray &buffer = socket->readAll();
        QString consulta=QString(buffer);
        json::JSON Obj = json::JSON::Load(consulta.toStdString());
        json::JSON respuesta;
        LinkedList lista=comuityque->getList();
        node *current = lista.getHead();
        if(Obj["command"].ToString()=="Get playlist"){
            respuesta["status"]="ok";
            respuesta["command"]="Get playlisit";
            while(current->getNextNode()){
                json::JSON song;
                song["id"]=current->getValueNode()->getId();
                song["song"]=current->getValueNode()->getId();
                song["guid"]=current->getValueNode()->getId();
                respuesta["playlist"].append(song);
                current=current->getNextNode();
            }
        }
        else if(Obj["command"].ToString()=="voteup"){
            respuesta["status"]="ok";
            respuesta["command"]="voteup";
            while(current->getNextNode()){
                if(current->getValueNode()->getId()==Obj["id"].ToString()){
                    current->getValueNode()->setUpVotes(current->getValueNode()->getUpVotes()+1);
                    break;
                }
                current=current->getNextNode();
            }
        }
        else if(Obj["command"].ToString()=="votedown"){
            respuesta["status"]="ok";
            respuesta["command"]="voteup";
            while(current->getNextNode()){
                if(current->getValueNode()->getId()==Obj["id"].ToString()){
                    current->getValueNode()->setDownVotes(current->getValueNode()->getDownVotes()+1);
                    break;
                }
                current=current->getNextNode();
            }
        }
        socket->write(respuesta.ToString().c_str());

    } else{
        json::JSON obj;
        obj["command"]="Get playlist";
        obj["status"]="no";
        socket->write(obj.ToString().c_str());

    }
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();
}
void MyTcpServer::communityTog() {
    this->comunitario=!comunitario;
}

void MyTcpServer::setComuityque(PriorityQueue *comuityque) {
    MyTcpServer::comuityque = comuityque;
}
