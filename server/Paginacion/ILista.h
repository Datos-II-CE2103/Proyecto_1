#ifndef ILISTA_H
#define ILISTA_H

#include <iostream>
#include "../estructuras_datos/cancion.h"

class ILista {
public:
    // Adds an element to the list
    virtual void add(const Cancion& value) = 0;

    // Deletes an element from the list
    virtual void remove(int index) = 0;

    // Searches for an element in the list
    virtual int search(const std::string& id) = 0;

    // Returns the element at the given index
    virtual Cancion getByIndex(int index) = 0;

    // Implements the LRU replacement algorithm
    virtual void lruReplace(const Cancion& value) = 0;
};

#endif // ILISTA_H