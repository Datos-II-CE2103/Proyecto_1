#ifndef NODO_H
#define NODO_H

#include "cancion.h"

class node {
private:
    Cancion* valueNode;
    node* nextNode;
    node* prevNode;

public:
    node();
    node(Cancion* newValue, node *n, node *p);
    node(Cancion* newValue);

    Cancion* getValueNode() const;
    node* getNextNode() const;
    node* getPrevNode() const;
    void setValueNode(Cancion* value);
    void setNextNode(node *next);
    void setPrevNode(node *prev);
    ~node();
};

#endif
