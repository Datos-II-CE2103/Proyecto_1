#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QMediaPlayer>
#include "tcp/tcpserver.h"
#include <QThread>
#include "estructuras_datos/doubly_linked_list.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void handlePlayBtn();
    void handlePauseBtn();
    void handleInfoText();
    //void handleMemory();
    void startTcp();
    void handleTcpConnections();

private:
    Ui::MainWindow *ui;
    QPushButton *playBtn,*pauseBtn;
    QTextEdit *infoTxt, *memoryTxt;
    QListWidget* listArt;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    MyTcpServer *socketEscucha;
    QThread *myThread;
    DoublyLinkedList listaCanciones;
};

#endif // MAINWINDOW_H

