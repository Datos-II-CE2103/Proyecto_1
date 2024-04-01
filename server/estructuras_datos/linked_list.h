#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "node.h"

class LinkedList {
private:
    nodo* head;
    nodo* tail;
    int size;

public:
    LinkedList();
    ~LinkedList();

    nodo* getHead() const;
    nodo* getTail() const;
    int getSize() const;
    bool isEmpty() const;
    void insertAtBeginning(Cancion* value);
    void insertAtEnd(Cancion* value);
    void insertBefore(nodo* nextNode, Cancion* value);
    void removeFromBeginning();
    void removeFromEnd();
    bool search(Cancion* value) const;
};

#endif // LINKED_LIST_H
