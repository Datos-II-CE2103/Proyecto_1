#include "mainwindow.h"
#include <thread>
#include <iostream>
#include <QApplication>
#include "headers/node.h"
#include "headers/linked_list.h"
#include "headers/inicializador.h"
#include <stdio.h>

using namespace std;


LinkedList* initial(){
    LinkedList* lista = new LinkedList();
    for (int i = 0; i < 5; i++) {
        lista->insertLast(i);
    }
    return lista;
}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
    /*LinkedList* inicial= initial();
    inicial->printList();
    inicial->removeFirst();
    inicial->printList();*/

    int listaEnteros[100];
    for(int i=0;i<100;i++){
        listaEnteros[i]=i;
    }


}
