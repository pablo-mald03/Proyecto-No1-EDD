#ifndef ARBOLAVL_H
#define ARBOLAVL_H

/*includes del arbol*/
#include"producto.h"
#include"nodoavl.h"


class ArbolAvl
{

private:
    NodoAvl* raiz;

    int obtenerAltura(NodoAvl* nodo);
    int obtenerFactorBalance(NodoAvl* nodo);

    /*Rotaciones del arbol*/
    NodoAvl* rotacionDerecha(NodoAvl* y);
    NodoAvl* rotacionIzquierda(NodoAvl* x);

    // Metodo recursivo de insercion (devuelve el nuevo nodo raiz del subarbol)
    NodoAvl* insertarRecursivo(NodoAvl* nodo, Producto producto);

    /*Metodo que permite evaluar si un producto ya existe con la misma llave primaria*/
    bool existeCodigoBarra(NodoAvl* nodo, const std::string& codigo);

public:
    ArbolAvl();
    ~ArbolAvl();

    /*Metodo que permite insertar un producto al arbol*/
    void insertar(Producto producto);
};

#endif // ARBOLAVL_H
