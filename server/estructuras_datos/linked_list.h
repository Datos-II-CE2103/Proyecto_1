#ifndef TSTQT_LINKED_LIST_H
#define TSTQT_LINKED_LIST_H
#include "node.h"
using namespace std;

class LinkedList{
private:
    nodo* head;
    nodo* tail;
    nodo* current;
    int *size;
public:
    ~LinkedList();
    LinkedList(nodo *headV, nodo *tailV, nodo *currentV, int *sizeV){
        head=headV;
        tail=tailV;
        current=currentV;
        size=sizeV;
    }
    LinkedList(){
        head= nullptr;
        tail= nullptr;
        current= nullptr;
        size=nullptr;
    }
    nodo *getHead() const {
        return head;
    }

    void setHead(nodo *head) {
        LinkedList::head = head;
    }

    nodo *getTail() const {
        return tail;
    }

    void setTail(nodo *tail) {
        LinkedList::tail = tail;
    }

    nodo *getCurrent() const {
        return current;
    }

    void setCurrent(nodo *current) {
        LinkedList::current = current;
    }

    int *getSize() const {
        return size;
    }

    void setSize(int *size) {
        LinkedList::size = size;
    }

    bool isEmpty(){
        return (this->head== nullptr);
    }

    void insertFirst(int element){
        nodo* newNode = new nodo(element);
        this->size++;

        if (this->isEmpty()){
            this->head  = this->head = newNode;
            this->current= this->head;
        } else{
            newNode->setNextNode(this->head);
            this->head->setPrevNode(newNode);
            this->head=newNode;
        }
    }
    void insertLast(int element){
        nodo* newNode = new nodo(element);
        this->size++;

        if (this->isEmpty()){
            this->head  = this->tail = newNode;
            this->current= this->head;
        } else{
            this->tail->setNextNode(newNode);
            newNode->setPrevNode(this->tail);
            this->tail=newNode;
        }
    }
    void removeFirst(){
        if (!this->isEmpty()){
            this->head=this->head->getNextNode();
            delete(this->head->getPrevNode());
            this->head->setPrevNode(nullptr);
            this->current=this->head;
            this->size--;
        }
    }
    void printList(){
        nodo* tempCurrent = this->current;
        while (tempCurrent!= nullptr){
            cout<<tempCurrent->getValueNode()<<" - ";
            tempCurrent=tempCurrent->getNextNode();
        }
        delete(tempCurrent);
    }
};

LinkedList::~LinkedList() {
}

#endif //TSTQT_LINKED_LIST_H
