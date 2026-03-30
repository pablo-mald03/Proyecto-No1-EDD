#ifndef ARBOLB_H
#define ARBOLB_H

#include "nodob.h"
class ArbolB
{

private:
    NodoB * raiz;

    int t;

    /*Metodo que permite dividir al hijo*/
    void dividirHijo(NodoB* padre, int i, NodoB* lleno);

    /*Metodo que permite insertar un nodo que no esta lleno*/
    void insertarNoLleno(NodoB* nodo, const Producto &producto);

    /*Metodo general para poder insertar*/
    void insertarNuevo(const Producto &producto);

    /*Metodo que permite validar integridad*/
    bool existeCodigo(NodoB* nodo, const std::string &codigo);

public:
    ArbolB(int _t);
    ~ArbolB();


    /*Metodo que permite insertar un nuevo producto en un arbol. Maneja la integridad*/
    void insertar(const Producto &producto);

};

#endif // ARBOLB_H
