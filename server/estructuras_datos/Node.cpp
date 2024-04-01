#include "node.h"

node::node() : valueNode(nullptr), nextNode(nullptr), prevNode(nullptr) {}

node::node(Cancion* newValue, node *n, node *p) : valueNode(newValue), nextNode(n), prevNode(p) {}

node::node(Cancion* newValue) : valueNode(newValue), nextNode(nullptr), prevNode(nullptr) {}

Cancion* node::getValueNode() const {
    return valueNode;
}

node* node::getNextNode() const {
    return nextNode;
}

node* node::getPrevNode() const {
    return prevNode;
}

void node::setValueNode(Cancion* value) {
    valueNode = value;
}

void node::setNextNode(node *next) {
    nextNode = next;
}

void node::setPrevNode(node *prev) {
    prevNode = prev;
}

node::~node() {
    // Aquí puedes agregar cualquier limpieza adicional necesaria en la destrucción de un node.
}
