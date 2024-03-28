#include <iostream>
#include "taglib/tag.h"
#include "taglib/fileref.h"
using namespace std;

int main() {
    //SE DEBE AÑADIR UNA RUTA DE ARCHIVO MUSICAL, USAR QTDir
    TagLib::FileRef f("../file.mp3");

    if(!f.isNull() && f.tag()) {

        TagLib::Tag *tag = f.tag();

        cout << "-- TAG (basic) LIST:--" << endl;
        cout << "title   - \"" << tag->title()   << "\"" << endl;
        cout << "artist  - \"" << tag->artist()  << "\"" << endl;
        cout << "album   - \"" << tag->album()   << "\"" << endl;
        cout << "year    - \"" << tag->year()    << "\"" << endl;
        cout << "comment - \"" << tag->comment() << "\"" << endl;
        cout << "track   - \"" << tag->track()   << "\"" << endl;
        cout << "genre   - \"" << tag->genre()   << "\"" << endl;
    } else{
        cout<<"no found"<<endl;
    }
    return 0;
}