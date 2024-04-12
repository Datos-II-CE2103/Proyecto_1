#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QMediaPlayer>
#include "tcp/tcpserver.h"
#include <QThread>
#include <QCheckBox>
#include "estructuras_datos/doubly_linked_list.h"
#include "estructuras_datos/node.h"

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
    void handleNextBtn();
    void handlePrevBtn();
    //void handleMemory();
    void startTcp();
    void handleTcpConnections();
    void updateInfoText();
    void playCurrentSong();
    void paginacion();
    void transferirNodos(QList<Cancion*>& cancionesPaginadas, node* inicio) ;

private:
    Ui::MainWindow *ui;
    QPushButton *playBtn,*pauseBtn, *nextBtn, *prevBtn;
    QTextEdit *infoTxt, *memoryTxt;
    QCheckBox *togglePaginacion;
    QListWidget* listArt;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    MyTcpServer *socketEscucha;
    QThread *myThread;
    DoublyLinkedList listaCanciones;
    bool isPaused;
    bool paginated;
    Cancion *currentSong;

    void cargarComunity();
};

#endif // MAINWINDOW_H

