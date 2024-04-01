#include "linked_list.h"

LinkedList::LinkedList() : head(nullptr), tail(nullptr), current(nullptr), size(0) {}

LinkedList::~LinkedList() {
    while (!isEmpty()) {
        removeFirst();
    }
}

node* LinkedList::getHead() const {
    return head;
}

void LinkedList::setHead(node* headPtr) {
    this->head = headPtr;
}

node* LinkedList::getTail() const {
    return tail;
}

void LinkedList::setTail(node* tailPtr) {
    this->tail = tailPtr;
}

node* LinkedList::getCurrent() const {
    return current;
}

void LinkedList::setCurrent(node* currentPtr) {
    this->current = currentPtr;
}

int LinkedList::getSize() const {
    return size;
}

bool LinkedList::isEmpty() const {
    return (head == nullptr);
}

void LinkedList::insertFirst(Cancion* element) {
    node* newNode = new node(element);

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
    node* newNode = new node(element);

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
        node* temp = head;
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
        node* temp = tail;
        tail = tail->getPrevNode();
        if (tail) {
            tail->setNextNode(nullptr);
        }
        delete temp;
        size--;
    }
}

void LinkedList::insertBefore(node* nextNode, Cancion* value) {
    node* newNode = new node(value);
    node* prevNode = nextNode->getPrevNode();

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
        node* temp = head;
        head = head->getNextNode();
        if (head) {
            head->setPrevNode(nullptr);
        }
        delete temp;
        size--;
    }
}
