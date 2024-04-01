// linked_list.h

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "node.h"
#include <iostream>

class LinkedList {
private:
    nodo* head;
    nodo* tail;
    nodo* current;
    int size;

public:
    LinkedList();
    ~LinkedList();

    nodo* getHead() const;
    void setHead(nodo* head);
    nodo* getTail() const;
    void setTail(nodo* tail);
    nodo* getCurrent() const;
    void setCurrent(nodo* current);
    int getSize() const;

    bool isEmpty() const;
    void insertFirst(Cancion* element);
    void insertLast(Cancion* element);
    void removeFirst();
    void removeLast();
};

#endif // LINKED_LIST_H
