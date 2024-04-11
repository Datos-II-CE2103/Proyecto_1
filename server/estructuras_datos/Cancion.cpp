#include "cancion.h"
#include <iostream>
#include <random>

Cancion::Cancion(const std::string& nombre, const std::string& artista, const std::string& album,
                 const std::string& genero, const std::string& archivoMP3)
        : nombre(nombre), artista(artista), album(album), genero(genero), upVotes(0), downVotes(0), archivoMP3(archivoMP3) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);

    const char hex_chars[] = "0123456789ABCDEF";
    const int length = 32;
    id.reserve(length);
    for (int i = 0; i < length; ++i) {
        if (i == 8 || i == 12 || i == 16 || i == 20) {
            id += '-';
        } else {
            id += hex_chars[dis(gen)];
        }
    }
}

std::string Cancion::getId() const {
    return id;
}

std::string Cancion::getNombre() const {
    return nombre;
}

void Cancion::setNombre(const std::string& nombre) {
    this->nombre = nombre;
}

std::string Cancion::getArtista() const {
    return artista;
}

void Cancion::setArtista(const std::string& artista) {
    this->artista = artista;
}

std::string Cancion::getAlbum() const {
    return album;
}

void Cancion::setAlbum(const std::string& album) {
    this->album = album;
}

std::string Cancion::getGenero() const {
    return genero;
}

void Cancion::setGenero(const std::string& genero) {
    this->genero = genero;
}

int Cancion::getUpVotes() const {
    return upVotes;
}

void Cancion::setUpVotes(int upVotes) {
    this->upVotes = upVotes;
}

int Cancion::getDownVotes() const {
    return downVotes;
}

void Cancion::setDownVotes(int downVotes) {
    this->downVotes = downVotes;
}

std::string Cancion::getArchivoMP3() const {
    return archivoMP3;
}

void Cancion::setArchivoMP3(const std::string& archivoMP3) {
    this->archivoMP3 = archivoMP3;
}

void Cancion::imprimirInfo() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Nombre: " << nombre << std::endl;
    std::cout << "Artista: " << artista << std::endl;
    std::cout << "Álbum: " << album << std::endl;
    std::cout << "Género: " << genero << std::endl;
    std::cout << "Up-votes: " << upVotes << std::endl;
    std::cout << "Down-votes: " << downVotes << std::endl;
    std::cout << "Archivo MP3: " << archivoMP3 << std::endl;
}
