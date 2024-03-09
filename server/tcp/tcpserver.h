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

class TcpServer : public QObject
{
Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

signals:

public slots:
    void newConnection();

private:
    TcpServer *server;
};

#endif //TSTQT_TCPSERVER_H
