#include "mainwindow.h"
#include <QApplication>
#include "tcp/tcpserver.h"
#include <glog/logging.h>
#include <string>
#include <filesystem>
#include <QDir>
#include <iostream>

using namespace std;

void printDir(){
    QString home=QDir::homePath();
    string carpeta=home.toStdString()+"/Music";
    for (auto &p : std::filesystem::directory_iterator(carpeta))
        std::cout << p << '\n';
}

int main(int argc, char *argv[])
{
    google::SetLogDestination(google::GLOG_INFO,"../log/" );
    google::InitGoogleLogging(argv[0]);
    QApplication a(argc, argv);

    printDir();

    //MyTcpServer server;

    MainWindow w;
    w.showMaximized();
    w.show();

    return a.exec();
}
