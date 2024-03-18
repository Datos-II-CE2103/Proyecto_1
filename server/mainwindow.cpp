#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPushButton>
#include <iostream>

#include <random>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    playBtn = ui->playBtn;

    infoTxt = ui->infoTxt;
    infoTxt->setText("Nombre Canción: \nNombre Artista: \nNombre Álbum: \nGénero: \nUp-Votes: \nDown-Votes: ");

    memoryTxt=ui->memoryTxt;

    std::random_device rd;
    std::mt19937 gen(rd());
    int min = 1;
    int max = 100;
    std::uniform_int_distribution<> dis(min, max);
    int random_number = dis(gen);
    memoryTxt->setText(QString::fromStdString(std::to_string(random_number)));

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
