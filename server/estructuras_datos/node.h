#ifndef NODO_H
#define NODO_H

class nodo {

private:
    int valueNode;
    nodo* nextNode;
    nodo* prevNode;

public:
    ~nodo();
    nodo(){
        valueNode=0;
        nextNode= nullptr;
        prevNode= nullptr;
    }
    nodo(int newValor, nodo *n, nodo *p)
    {
        valueNode=newValor;
        nextNode=n;
        prevNode=p;
    }
    nodo(int newValor)
    {
        valueNode=newValor;
        nextNode= nullptr;
        prevNode= nullptr;
    }

    int getValueNode() const {
        return valueNode;
    }

    nodo *getNextNode() const {
        return nextNode;
    }

    nodo *getPrevNode() const {
        return prevNode;
    }

    void setValueNode(int valueNode) {
        nodo::valueNode = valueNode;
    }

    void setNextNode(nodo *nextNode) {
        nodo::nextNode = nextNode;
    }

    void setPrevNode(nodo *prevNode) {
        nodo::prevNode = prevNode;
    }
};

nodo::~nodo() {
}

#endif