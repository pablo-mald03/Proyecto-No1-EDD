#ifndef NODOLISTA_H
#define NODOLISTA_H

/*Includes de la clase*/
#include"producto.h"

class NodoLista
{

public:
    NodoLista(const Producto &producto);
    ~NodoLista();

    /*Metodos getters y setters*/
    Producto& getDato();

    NodoLista* getSiguiente() const;
    NodoLista* getAnterior() const;

    void setSiguiente(NodoLista* _siguiente);
    void setAnterior(NodoLista* _anterior);


private:
    Producto dato;
    NodoLista * siguiente;
    NodoLista * anterior;



};

#endif // NODOLISTA_H
