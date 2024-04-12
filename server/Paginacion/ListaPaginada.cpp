#include "ListaPaginada.h"
#include <iostream>

ListaPaginada::ListaPaginada(int capacity) : capacity(capacity) {}

void ListaPaginada::add(const Cancion& value) {
    if (elements.size() == capacity) {
        lruReplace(value);
    } else {
        elements.push_front(value);
        index[value.getId()] = elements.begin();
    }
}

void ListaPaginada::remove(int idx) {
    if (idx >= 0 && idx < elements.size()) {
        auto it = std::next(elements.begin(), idx);
        it = elements.erase(it);
        index.erase(it->getId());
    } else {
        std::cout << "Índice inválido" << std::endl;
    }
}

int ListaPaginada::search(const std::string& id) {
    auto it = index.find(id);
    if (it != index.end()) {
        elements.splice(elements.begin(), elements, it->second);
        index[id] = elements.begin();
        return 1;
    }
    return 0;
}

Cancion ListaPaginada::getByIndex(int idx) {
    if (idx >= 0 && idx < elements.size()) {
        auto it = std::next(elements.begin(), idx);
        return *it;
    }
    return Cancion();
}

void ListaPaginada::lruReplace(const Cancion& value) {
    if (index.size() == capacity) {
        auto it = elements.end();
        --it;
        index.erase(it->getId());
        elements.pop_back();
    }
    elements.push_front(value);
    index[value.getId()] = elements.begin();
}
