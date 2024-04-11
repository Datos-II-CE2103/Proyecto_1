#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "linked_list.h"
#include "cancion.h"

class PriorityQueue {
private:
    LinkedList list;
    int maxSize;

public:
    PriorityQueue();
    ~PriorityQueue();

    void insert(Cancion* song);
    void remove();
    bool isEmpty() const;
    bool isFull() const;
};

#endif // PRIORITY_QUEUE_H
