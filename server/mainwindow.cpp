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
#include "estructuras_datos/doubly_linked_list.h"
#include "estructuras_datos/cancion.h"
#include <filesystem>

using namespace std;

void cargarCanciones(DoublyLinkedList& lista) {

    QSettings carpetaMusica("../settings/server.ini", QSettings::Format::IniFormat);
    std::string rutaMusica = carpetaMusica.value("rutas/rutabibliotecamusical").toString().toStdString();

    for (const auto & entry : std::filesystem::directory_iterator(rutaMusica)) {
        if (entry.path().has_extension() && entry.path().extension() == ".mp3") {
            Cancion* nuevaCancion = new Cancion();
            nuevaCancion->setTagsFromMP3(entry.path().string());
            lista.insertLast(nuevaCancion);
        }
    }
}

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
    cout<<player->hasAudio();
    audioOutput->setVolume(50);

    playBtn = ui->playBtn;
    pauseBtn = ui->pauseBtn;
    nextBtn = ui->nextBtn;
    prevBtn = ui->backBtn;

    infoTxt = ui->infoTxt;
    infoTxt->setText("Nombre Canción: \nNombre Artista: \nNombre Álbum: \nGénero: \nUp-Votes: \nDown-Votes: ");

    memoryTxt=ui->memoryTxt;

    unsigned long long memory_used = getMemoryUsage();

    memoryTxt->setText(QString::fromStdString(std::to_string(memory_used)));

    listArt = ui->listArt;
    listArt->addItem("Feid");
    listArt->addItem("Bad Bunny");
    listArt->addItem("Karol G");

    // Cargar las canciones desde la carpeta de música
    cargarCanciones(listaCanciones);

    //listaCanciones.printAllSongs();
    //listaCanciones.getHead()->getValueNode()->imprimirInfo();

    connect(playBtn, &QPushButton::clicked, this, &MainWindow::handlePlayBtn);
    connect(socketEscucha, SIGNAL(nuevaConex()), this, SLOT(handleTcpConnections()));
    connect(pauseBtn, &QPushButton::clicked, this, &MainWindow::handlePauseBtn);
    connect(nextBtn, &QPushButton::clicked, this, &MainWindow::handleNextBtn);
    connect(prevBtn, &QPushButton::clicked, this, &MainWindow::handlePrevBtn);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::updateInfoText);

    currentSong = nullptr;
}

void MainWindow::handlePlayBtn() {

    node* firstNode = listaCanciones.getHead();

    if (firstNode && !isPaused) {
        Cancion* primeraCancion = firstNode->getValueNode();

        QSettings carpetaMusica("../settings/server.ini", QSettings::Format::IniFormat);
        QString rutaMusica = carpetaMusica.value("rutas/rutabibliotecamusical").toString();
        std::string archivoMP3 = primeraCancion->getArchivoMP3();
        QString rutaCompleta = rutaMusica + "/" + QString::fromStdString(archivoMP3);

        player->setSource(QUrl::fromLocalFile(rutaCompleta));
        player->play();

        currentSong = primeraCancion;
    }
    else{
        player->play();
        isPaused=false;
    }
}

void MainWindow::handlePauseBtn() {
    if (!isPaused){
        player->pause();
        isPaused= true;
        currentSong = nullptr;
    }
}
void MainWindow::handleNextBtn() {

    node* currentNode = listaCanciones.getCurrent();

    if (!currentNode) {
        return;
    }
    listaCanciones.moveToNext(currentNode);
    Cancion* currentSong = currentNode->getValueNode();
    //currentNode->getValueNode()->imprimirInfo();

    QSettings carpetaMusica("../settings/server.ini", QSettings::Format::IniFormat);
    QString rutaMusica = carpetaMusica.value("rutas/rutabibliotecamusical").toString();
    std::string archivoMP3 = currentSong->getArchivoMP3();
    QString rutaCompleta = rutaMusica + "/" + QString::fromStdString(archivoMP3);

    player->setSource(QUrl::fromLocalFile(rutaCompleta));
    player->play();

    this->currentSong = currentSong;

    currentSong->imprimirInfo();
}
void MainWindow::handlePrevBtn() {
    node* currentNode = listaCanciones.getCurrent();
    if (!currentNode){
        return;
    }
    listaCanciones.moveToPrev(currentNode);
    Cancion* currentSong = currentNode->getValueNode();

    QSettings carpetaMusica("../settings/server.ini", QSettings::Format::IniFormat);
    QString rutaMusica = carpetaMusica.value("rutas/rutabibliotecamusical").toString();
    std::string archivoMP3 = currentSong->getArchivoMP3();
    QString rutaCompleta = rutaMusica + "/" + QString::fromStdString(archivoMP3);

    player->setSource(QUrl::fromLocalFile(rutaCompleta));
    player->play();

    this->currentSong = currentSong;

    currentSong->imprimirInfo();
}
void MainWindow::updateInfoText() {
    if (currentSong) {
        infoTxt->setText("Nombre Canción: " + QString::fromStdString(currentSong->getNombre()) + "\n"
                         + "Nombre Artista: " + QString::fromStdString(currentSong->getArtista()) + "\n"
                         + "Nombre Álbum: " + QString::fromStdString(currentSong->getAlbum()) + "\n"
                         + "Género: " + QString::fromStdString(currentSong->getGenero()) + "\n"
                         + "Up-Votes: " + QString::number(currentSong->getUpVotes()) + "\n"
                         + "Down-Votes: " + QString::number(currentSong->getDownVotes()));
    }
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
