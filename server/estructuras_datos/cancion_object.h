#ifndef CANCION_H
#define CANCION_H

#include <string>
#include <iostream>

class Cancion {
private:
    std::string id;
    std::string nombre;
    std::string artista;
    std::string album;
    std::string genero;
    int upVotes;
    int downVotes;
    std::string archivoMP3;

public:
    Cancion(const std::string& nombre, const std::string& artista, const std::string& album,
            const std::string& genero, const std::string& archivoMP3);

    std::string getId() const;

    std::string getNombre() const;

    void setNombre(const std::string& nombre);

    std::string getArtista() const;

    void setArtista(const std::string& artista);

    std::string getAlbum() const;

    void setAlbum(const std::string& album);

    std::string getGenero() const;

    void setGenero(const std::string& genero);

    int getUpVotes() const;

    void setUpVotes(int upVotes);

    int getDownVotes() const;

    void setDownVotes(int downVotes);

    std::string getArchivoMP3() const;

    void setArchivoMP3(const std::string& archivoMP3);

    void imprimirInfo() const;
};

#endif // CANCION_H

