#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QPushButton>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    playBtn = ui->playBtn;

    infoTxt = ui->infoTxt;

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


