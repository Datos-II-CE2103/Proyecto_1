#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <iostream>
#include <fstream>
#include <string>
#include <QDir>
#include <QThread>


using namespace std;

unsigned long long getMemoryUsage() {
    std::ifstream file("/proc/self/status");
    std::string line;
    unsigned long long memory_usage = 0;

    while (std::getline(file, line)) {
        if (line.substr(0, 6) == "VmSize") {
            memory_usage = std::stoull(line.substr(7));
            break;
        }
    }

    return memory_usage;
}

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startTcp();
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    QString home=QDir::homePath();
    player->setSource(QUrl::fromLocalFile(home+"/Music/NinaBonita.mp3"));
    cout<<player->hasAudio();
    audioOutput->setVolume(50);

    playBtn = ui->playBtn;

    infoTxt = ui->infoTxt;
    infoTxt->setText("Nombre Canción: \nNombre Artista: \nNombre Álbum: \nGénero: \nUp-Votes: \nDown-Votes: ");

    memoryTxt=ui->memoryTxt;

    unsigned long long memory_used = getMemoryUsage();

    memoryTxt->setText(QString::fromStdString(std::to_string(memory_used)));

    listArt = ui->listArt;
    listArt->addItem("Feid");
    listArt->addItem("Bad Bunny");
    listArt->addItem("Karol G");

    connect(playBtn, SIGNAL(clicked()), this, SLOT(handlePlayBtn()));
    connect(playBtn, SIGNAL(clicked()), this, SLOT(handleInfoText()));
    //connect(socketEscucha, SIGNAL(newConnection()),this,SLOT(handlePlayBtn()));
    connect(socketEscucha, SIGNAL(nuevaConex()), this, SLOT(handleTcpConnections()));
}

void MainWindow::handlePlayBtn() {
    player->play();
    std::cout << "Play" << std::endl;
}
void MainWindow::handlePauseBtn() {
    player->pause();
    std::cout << "Pause" << std::endl;
}
void MainWindow::handleInfoText() {
    infoTxt->setText("Hola");
}
void MainWindow::startTcp() {
    myThread= new QThread;
    socketEscucha= new MyTcpServer;
    socketEscucha->moveToThread(myThread);
    myThread->start();
}
void MainWindow::handleTcpConnections() {
            player->play();
}

MainWindow::~MainWindow()
{
    delete ui;
}
