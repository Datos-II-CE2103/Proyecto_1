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
#include <glog/logging.h>
#include <QList>

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

    togglePaginacion = ui->TogglePaginacion;

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
    connect(togglePaginacion, &QCheckBox::stateChanged, this, &MainWindow::paginacion);
    currentSong = nullptr;
}

void MainWindow::handlePlayBtn() {

    node* firstNode = listaCanciones.getHead();

    if (firstNode){
        if (!isPaused) {
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
    } else {
        LOG(ERROR) << "No hay una canción que esté primera";
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

    if (currentNode && currentNode->getNextNode()){
        listaCanciones.moveToNext(currentNode);
        playCurrentSong();
    } else {
        LOG(ERROR) << "No hay una canción siguiente disponible";
        return;
    }
}
void MainWindow::handlePrevBtn() {
    node* currentNode = listaCanciones.getCurrent();

    if(currentNode && currentNode->getPrevNode()){
        listaCanciones.moveToPrev(currentNode);
        playCurrentSong();
    } else{
        LOG(ERROR) << "No hay una canción anterior disponible";
        return;
    }
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
void MainWindow::playCurrentSong(){
    node* currentNode = listaCanciones.getCurrent();
    if (currentNode){
        Cancion* currentSong = currentNode->getValueNode();

        QSettings carpetaMusica("../settings/server.ini", QSettings::Format::IniFormat);
        QString rutaMusica = carpetaMusica.value("rutas/rutabibliotecamusical").toString();
        std::string archivoMP3 = currentSong->getArchivoMP3();
        QString rutaCompleta = rutaMusica + "/" + QString::fromStdString(archivoMP3);

        player->setSource(QUrl::fromLocalFile(rutaCompleta));
        player->play();

        this->currentSong = currentSong;

        currentSong->imprimirInfo();
    } else {
        LOG(ERROR) << "No se pudo obtener la canción actual";
        return;
    }
}

void MainWindow::transferirNodos(QList<Cancion*>& cancionesPaginadas, node* inicio) {
    node* currentNode = inicio;
    while (currentNode) {
        cancionesPaginadas.append(currentNode->getValueNode());
        currentNode = currentNode->getNextNode();
    }
}

void MainWindow::paginacion() {
    if (!paginated) {
        QList<Cancion*> cancionesPaginadas;
        transferirNodos(cancionesPaginadas, listaCanciones.getHead());
                foreach(Cancion* cancion, cancionesPaginadas) {
                std::cout << cancion->getNombre() << std::endl;
            }
        paginated = true;
    } else {
        paginated = false;
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
