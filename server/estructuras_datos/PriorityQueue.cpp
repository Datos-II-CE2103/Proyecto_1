#include "PriorityQueue.h"
#include <iostream>

PriorityQueue::PriorityQueue() : maxSize(10) {}

PriorityQueue::~PriorityQueue() {}

void PriorityQueue::insert(Cancion* song) {
    if (isFull()) {
        std::cout << "La cola de prioridad está llena. No se puede insertar más canciones." << std::endl;
        return;
    }

    int effectiveVotes = song->getUpVotes() - song->getDownVotes();

    node* current = list.getHead();

    while (current && effectiveVotes <= (current->getValueNode()->getUpVotes() - current->getValueNode()->getDownVotes())) {
        current = current->getNextNode();
    }

    if (!current) {
        list.insertLast(song);
    } else {
        list.insertBefore(current, song);
    }
}

void PriorityQueue::remove() {
    if (isEmpty()) {
        std::cout << "La cola de prioridad está vacía. No se puede eliminar ninguna canción." << std::endl;
        return;
    }

    list.removeFromBeginning();
}

bool PriorityQueue::isEmpty() const {
    return list.isEmpty();
}

bool PriorityQueue::isFull() const {
    return list.getSize() >= maxSize;
}

LinkedList PriorityQueue::getList() {
    return LinkedList();
}
