#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <iostream>
#include <fstream>
#include <string>

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
}

void MainWindow::handlePlayBtn() {
    std::cout << "Play" << std::endl;
}

void MainWindow::handleInfoText() {
    infoTxt->setText("Hola");
}

MainWindow::~MainWindow()
{
    delete ui;
}
