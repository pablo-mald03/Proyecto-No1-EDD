#include "nodolista.h"

NodoLista::NodoLista(const Producto &producto):dato(producto)
{

}

/*Destructor*/
NodoLista::~NodoLista(){

}

/*Metodos getter y setter*/

Producto& NodoLista::getDato(){
    return dato;
}


/*Metodos getters y setters para reutilizar nodo*/
NodoLista* NodoLista::getSiguiente() const{
    return this->siguiente;
}

NodoLista* NodoLista::getAnterior() const{
    return this->anterior;
}


void NodoLista::setSiguiente(NodoLista* _siguiente){
    this->siguiente = _siguiente;
}


void NodoLista::setAnterior(NodoLista* _anterior){
    this->anterior = _anterior;
}

