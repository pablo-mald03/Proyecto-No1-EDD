#include "arbolavl.h"
#include "insertexception.h"
#include <algorithm>

/*Constructor del arbol Avl*/
ArbolAvl::ArbolAvl(): raiz(nullptr)
{

}

/*Metodo que permite obtener la altura del nodo*/
int ArbolAvl::obtenerAltura(NodoAvl* nodo){

    if(nodo == nullptr){
        return 0;
    }

    return nodo->getAltura();
}

/*Metodo que permite obtener el factor de balance del nodo*/
int ArbolAvl::obtenerFactorBalance(NodoAvl* nodo){
    if(nodo == nullptr){
        return 0;
    }

    return obtenerAltura(nodo->getIzquierda()) - obtenerAltura(nodo->getDerecha());
}

/*Metodo que permite generar la rotacion hacia la derecha de los nodos*/
/*Si y se desbalanceo hacia la derecha se baja y y se sube x*/
/*
 * se retorna la raiz del subarbol
 */
NodoAvl* ArbolAvl::rotacionDerecha(NodoAvl* y){

    NodoAvl * x = y->getIzquierda();
    NodoAvl * tier2 = x->getDerecha();

    x->setDerecha(y);
    y->setIzquierda(tier2);

    y->setAltura( std::max(obtenerAltura(y->getIzquierda()), obtenerAltura(y->getDerecha())) + 1);
    x->setAltura( std::max(obtenerAltura(x->getIzquierda()), obtenerAltura(x->getDerecha())) + 1);

    return x;
}

/*Metodo que permite generar la rotacion hacia la izquierda de los nodos*/
/*Si x se desbalanceo hacia la derecha se baja x y se sube y*/
/*
 * se retorna la raiz del subarbol
 */
NodoAvl* ArbolAvl::rotacionIzquierda(NodoAvl * x){
    NodoAvl* y= x->getDerecha();
    NodoAvl* tier2 = y->getIzquierda();

    y->setIzquierda(x);
    x->setDerecha(tier2);

    x->setAltura(std::max(obtenerAltura(x->getIzquierda()), obtenerAltura(x->getDerecha())) + 1);

    y->setAltura(std::max(obtenerAltura(y->getIzquierda()), obtenerAltura(y->getDerecha())) + 1);

    return y;
}

/*Metodo utilizado para generar la insercion en el arbol avl*/
void ArbolAvl::insertar(Producto producto){

    /*Verificacion de integridad*/
    if (this->existeCodigoBarra(this->raiz, producto.getCodigoBarra())) {
        throw InsertException("El codigo de barras { " +
                              producto.getCodigoBarra() + " } ya existe para el producto: " +
                              producto.getNombre() + " [ARBOL AVL]");
    }

    this->raiz = insertarRecursivo(this->raiz,producto);
}

/*Metodo recursivo que permite insertar el valor en el nodo del arbol*/
/*
* La gracia esta en que siempre retorna a la raiz nueva
*/
NodoAvl* ArbolAvl::insertarRecursivo(NodoAvl * nodo, Producto producto){
    if(nodo == nullptr){
        return new NodoAvl(producto);
    }

    if(producto.getNombre() < nodo->getDato().getNombre()){

        nodo->setIzquierda(insertarRecursivo(nodo->getIzquierda(),producto));
    }
    else{
        nodo->setDerecha(insertarRecursivo(nodo->getDerecha(),producto));
    }

    nodo->setAltura(1 + std::max(obtenerAltura(nodo->getIzquierda()), obtenerAltura(nodo->getDerecha())));

    int balance = obtenerFactorBalance(nodo);

    /*Genera rotacion izquierda-izquierda*/
    if(balance > 1 && producto.getNombre() < nodo->getIzquierda()->getDato().getNombre()){
        return rotacionDerecha(nodo);
    }

    /*Genera rotacion derecha-derecha*/
    if(balance < -1 && producto.getNombre() >= nodo->getDerecha()->getDato().getNombre()){
        return rotacionIzquierda(nodo);
    }

    /*Genera rotacion izquireda-derecha*/
    if(balance > 1 && producto.getNombre() >= nodo->getIzquierda()->getDato().getNombre()){
        nodo->setIzquierda( rotacionIzquierda(nodo->getIzquierda()));
        return rotacionDerecha(nodo);
    }

    /*Genera rotacion derecha-izquierda*/
    if(balance < -1 && producto.getNombre() < nodo->getDerecha()->getDato().getNombre()){
        nodo->setDerecha(rotacionDerecha(nodo->getDerecha()));
        return rotacionIzquierda(nodo);
    }

    /*Si esta balanceado solo retorna*/
    return nodo;

}

/*Metodo que permite evaluar si existe duplicidad*/
bool ArbolAvl::existeCodigoBarra(NodoAvl* nodo, const std::string& codigo){
    if (nodo == nullptr){
        return false;
    }

    if (nodo->getDato().getCodigoBarra() == codigo) {
        return true;
    }

    return existeCodigoBarra(nodo->getIzquierda(), codigo) ||
           existeCodigoBarra(nodo->getDerecha(), codigo);
}

