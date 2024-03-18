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

    connect(playBtn, SIGNAL(clicked()), this, SLOT(handlePlayBtn()));
}

void MainWindow::handlePlayBtn() {
    std::cout << "Play" << std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}


