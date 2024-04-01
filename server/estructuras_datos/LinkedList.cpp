#include "linked_list.h"
#include <iostream>

LinkedList::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

LinkedList::~LinkedList() {
    while (!isEmpty()) {
        removeFromBeginning();
    }
}

nodo* LinkedList::getHead() const {
    return head;
}

nodo* LinkedList::getTail() const {
    return tail;
}

int LinkedList::getSize() const {
    return size;
}

bool LinkedList::isEmpty() const {
    return head == nullptr;
}

void LinkedList::insertAtBeginning(Cancion* value) {
    nodo* newNode = new nodo(value);
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        newNode->setNextNode(head);
        head->setPrevNode(newNode);
        head = newNode;
    }
    size++;
}

void LinkedList::insertAtEnd(Cancion* value) {
    nodo* newNode = new nodo(value);
    if (isEmpty()) {
        head = tail = newNode;
    } else {
        tail->setNextNode(newNode);
        newNode->setPrevNode(tail);
        tail = newNode;
    }
    size++;
}

void LinkedList::insertBefore(nodo* nextNode, Cancion* value) {
    nodo* newNode = new nodo(value);
    nodo* prevNode = nextNode->getPrevNode();

    newNode->setNextNode(nextNode);
    newNode->setPrevNode(prevNode);

    if (prevNode) {
        prevNode->setNextNode(newNode);
    } else {
        head = newNode;
    }
    nextNode->setPrevNode(newNode);

    size++;
}

void LinkedList::removeFromBeginning() {
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

void LinkedList::removeFromEnd() {
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

bool LinkedList::search(Cancion* value) const {
    nodo* current = head;
    while (current) {
        if (current->getValueNode() == value) {
            return true;
        }
        current = current->getNextNode();
    }
    return false;
}
