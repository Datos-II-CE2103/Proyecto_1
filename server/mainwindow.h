#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QMediaPlayer>

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

private:
    Ui::MainWindow *ui;
    QPushButton* playBtn;
    QPushButton* pauseBtn;
    QTextEdit* infoTxt;
    QTextEdit* memoryTxt;
    QListWidget* listArt;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
};

#endif // MAINWINDOW_H

