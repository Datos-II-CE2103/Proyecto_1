#ifndef NODO_H
#define NODO_H

#include "cancion.h"

class nodo {
private:
    Cancion* valueNode;
    nodo* nextNode;
    nodo* prevNode;

public:
    ~nodo();
    nodo(){
        valueNode = nullptr;
        nextNode = nullptr;
        prevNode = nullptr;
    }
    nodo(Cancion* newValue, nodo *n, nodo *p)
    {
        valueNode = newValue;
        nextNode = n;
        prevNode = p;
    }
    nodo(Cancion* newValue)
    {
        valueNode = newValue;
        nextNode = nullptr;
        prevNode = nullptr;
    }

    Cancion* getValueNode() const {
        return valueNode;
    }

    nodo *getNextNode() const {
        return nextNode;
    }

    nodo *getPrevNode() const {
        return prevNode;
    }

    void setValueNode(Cancion* value) {
        valueNode = value;
    }

    void setNextNode(nodo *next) {
        nextNode = next;
    }

    void setPrevNode(nodo *prev) {
        prevNode = prev;
    }
};

nodo::~nodo() {
}

#endif
