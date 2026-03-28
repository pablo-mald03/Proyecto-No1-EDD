#include "gestorestructuras.h"

GestorEstructuras::GestorEstructuras():
    listaNoOrdenada(new ListaEnlazada()),
    listaOrdenada(new ListaEnlazada())
{

}

/*Destructor*/
GestorEstructuras::~GestorEstructuras(){
    if(this->listaNoOrdenada != nullptr){
        delete this->listaNoOrdenada;
    }

    if(this->listaOrdenada != nullptr){
        delete this->listaOrdenada;
    }
}


/*Metodo que permite insertar los datos en la lista*/
void GestorEstructuras::insertarListas(std::string nombre, std::string key, std::string categoria, std::string fecha, std::string marca, double precio, int stock){

    this->listaNoOrdenada->insertarAtras(Producto(nombre,key,categoria,fecha,marca,precio,stock));

}
