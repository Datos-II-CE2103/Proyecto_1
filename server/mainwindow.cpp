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
    QString home = QDir::homePath();
    string carpeta = home.toStdString() + "/Music";

    for (const auto& entry : std::filesystem::directory_iterator(carpeta)) {
        if (entry.path().extension() == ".mp3") {
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
    QString home=QDir::homePath();
    //player->setSource(QUrl::fromLocalFile(home+"/Music/file.mp3"));
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

    // Cargar las canciones desde la carpeta de música
    cargarCanciones(listaCanciones);

    connect(playBtn, &QPushButton::clicked, this, &MainWindow::handlePlayBtn);
    connect(socketEscucha, SIGNAL(nuevaConex()), this, SLOT(handleTcpConnections()));
}

void MainWindow::handlePlayBtn() {

    node* firstNode = listaCanciones.getHead();

    if (firstNode) {

        Cancion* primeraCancion = firstNode->getValueNode();

        QSettings carpetaMusica("../settings/server.ini", QSettings::Format::IniFormat);
        QString rutaMusica = carpetaMusica.value("rutas/rutabibliotecamusical").toString();

        std::string archivoMP3 = primeraCancion->getArchivoMP3();

        QString rutaCompleta = rutaMusica + "/" + QString::fromStdString(archivoMP3);
        player->setSource(QUrl::fromLocalFile(rutaCompleta));

        player->play();

        // Mostrar el nombre de la canción en la información de texto
        infoTxt->setText("Nombre Canción: " + QString::fromStdString(primeraCancion->getNombre()) + "\n"
                         + "Nombre Artista: " + QString::fromStdString(primeraCancion->getArtista()) + "\n"
                         + "Nombre Álbum: " + QString::fromStdString(primeraCancion->getAlbum()) + "\n"
                         + "Género: " + QString::fromStdString(primeraCancion->getGenero()) + "\n"
                         + "Up-Votes: " + QString::number(primeraCancion->getUpVotes()) + "\n"
                         + "Down-Votes: " + QString::number(primeraCancion->getDownVotes()));
    }
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
