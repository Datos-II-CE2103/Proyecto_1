#include "mainwindow.h"
#include <QApplication>
#include <glog/logging.h>
#include <QDir>

using namespace std;

int main(int argc, char *argv[])
{
    google::SetLogDestination(google::GLOG_INFO,"../log/" );
    google::InitGoogleLogging(argv[0]);
    QApplication a(argc, argv);

    //MyTcpServer server;

    MainWindow w;
    w.showMaximized();
    w.show();

    return a.exec();
}
