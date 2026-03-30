#ifndef ARBOLBMAS_H
#define ARBOLBMAS_H

/*Includes de la clase*/
#include "nodobmas.h"

class ArbolBMas
{
private:
    NodoBMas* raiz;

    /*Orden seteable*/
    int orden;

    /*Metodo que permite insertar datos en un nodo del arbol*/
    void insertarEnNodo(NodoBMas* nodo, const Producto &producto);

    /*Metodo que permite generar la division de las hojas cuando se sobrepasa el limite*/
    void dividirHoja(NodoBMas* hoja, NodoBMas* padre, int indiceEnPadre);

    /*Metodo que permite dividir la lista cuando se genera el desbordamiento*/
    void dividirInterno(NodoBMas* interno, NodoBMas* padre, int indiceEnPadre);

    /*Metodo que permite generar la recursividad para poder insertar en el lugar correcto*/
    void insertarRecursivo(NodoBMas *nodo, NodoBMas *padre, Producto producto, int indiceEnPadre);

    /*Metodo que permite conservar la integridad del arbol buscando la primary key que es el codigo de barra*/
    bool yaExiste(NodoBMas* nodoRaiz, const Producto &producto);

public:
    ArbolBMas(int _orden);
    ~ArbolBMas();

    /*Metodo que permite insertar un nuevo producto*/
    void insertar(const Producto &producto);
};

#endif // ARBOLBMAS_H
