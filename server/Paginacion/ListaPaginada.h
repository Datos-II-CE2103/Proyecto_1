#ifndef LISTA_H
#define LISTA_H

#include "ILista.h"
#include <list>
#include <unordered_map>

class ListaPaginada : public ILista {
private:
    std::list<Cancion> elements;
    std::unordered_map<std::string, std::list<Cancion>::iterator> index;
    int capacity;

public:
    ListaPaginada(int capacity);

    void add(const Cancion& value) override;

    void remove(int index) override;

    int search(const std::string& id) override;

    Cancion getByIndex(int index) override;

    void lruReplace(const Cancion& value) override;
};

#endif