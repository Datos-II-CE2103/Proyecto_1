#include "doubly_linked_list.h"
#include <iostream>

LinkedList::LinkedList() : head(nullptr), tail(nullptr), current(nullptr), size(0) {}

LinkedList::~LinkedList() {
    while (!isEmpty()) {
        removeFirst();
    }
}

nodo* LinkedList::getHead() const {
    return head;
}

void LinkedList::setHead(nodo* head) {
    this->head = head;
}

nodo* LinkedList::getTail() const {
    return tail;
}

void LinkedList::setTail(nodo* tail) {
    this->tail = tail;
}

nodo* LinkedList::getCurrent() const {
    return current;
}

void LinkedList::setCurrent(nodo* current) {
    this->current = current;
}

int LinkedList::getSize() const {
    return size;
}

bool LinkedList::isEmpty() const {
    return (head == nullptr);
}

void LinkedList::insertFirst(Cancion* element) {
    nodo* newNode = new nodo(element);

    if (isEmpty()) {
        head = tail = newNode;
    } else {
        newNode->setNextNode(head);
        head->setPrevNode(newNode);
        head = newNode;
    }

    size++;
}

void LinkedList::insertLast(Cancion* element) {
    nodo* newNode = new nodo(element);

    if (isEmpty()) {
        head = tail = newNode;
    } else {
        newNode->setPrevNode(tail);
        tail->setNextNode(newNode);
        tail = newNode;
    }

    size++;
}

void LinkedList::removeFirst() {
    if (!isEmpty()) {
        nodo* temp = head;
        head = head->getNextNode();
        if (head) {
            head->setPrevNode(nullptr);
        }
        delete temp;
        size--;
    }
}

void LinkedList::removeLast() {
    if (!isEmpty()) {
        nodo* temp = tail;
        tail = tail->getPrevNode();
        if (tail) {
            tail->setNextNode(nullptr);
        }
        delete temp;
        size--;
    }
}
