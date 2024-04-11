//
// Created by alexander on 9/03/24.
//

#ifndef TSTQT_TCPSERVER_H
#define TSTQT_TCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QSettings>
#include <QString>
#include <QDebug>

class MyTcpServer : public QObject
{
Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);

signals:
    void nuevaConex();

public slots:
    void newConnection();

private:
    QTcpServer *server;
};

#endif //TSTQT_TCPSERVER_H
