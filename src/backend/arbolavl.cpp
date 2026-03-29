#include "arbolavl.h"
#include "deleteexception.h"
#include "insertexception.h"
#include <algorithm>

/*Constructor del arbol Avl*/
ArbolAvl::ArbolAvl(): raiz(nullptr)
{

}

/*Destructor*/
ArbolAvl::~ArbolAvl(){

    vaciarArbol(this->raiz);
    this->raiz = nullptr;
}

/*Metodo que permite retornar la referencia a la raiz*/
NodoAvl* ArbolAvl::getRaiz(){
    return this->raiz;
}

/*Metodo que permite vaciar el arbol por completo. Borra rama por rama*/
void ArbolAvl::vaciarArbol(NodoAvl* nodo){

    if(nodo == nullptr){
        return;
    }

    vaciarArbol(nodo->getIzquierda());
    vaciarArbol(nodo->getDerecha());

    delete nodo;
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

/*Metodo que permite listar en orden alfabetico el arbol*/
/*
*   Permite recorrer lo mas a la izquierda posible que es donde estan los primeros alfabeticamente
*/
void ArbolAvl::listarProductosInOrden(NodoAvl* nodo, ListaEnlazada<Producto>* lista){

    if(nodo == nullptr){
        return;
    }

    listarProductosInOrden(nodo->getIzquierda(),lista);

    lista->insertarAtras(nodo->getDato());

    listarProductosInOrden(nodo->getDerecha(),lista);
}


/*Metodo que permite generar la busqueda del producto por nombre*/
/*
* Genera una busqueda binaria
*/
Producto* ArbolAvl::buscarProducto(NodoAvl* nodo, const std::string& nombre){

    if(nodo == nullptr){
        return nullptr;
    }

    if(nombre == nodo->getDato().getNombre()){
        return &(nodo->getDato());
    }

    if(nombre < nodo->getDato().getNombre()){
        return buscarProducto(nodo->getIzquierda(),nombre);
    }
    else{
        return buscarProducto(nodo->getDerecha(),nombre);
    }

}

/*---****----Apartado de metodos utilizados para poder eliminar nodos del arbol---****---*/

/*Metodo publico que permite eliminar en el arbol*/
void ArbolAvl::eliminarPorCodigo(const std::string &codigo){

    Producto* productoEliminar = buscarPorCodigobarra(this->raiz,codigo);

    if(productoEliminar == nullptr){

        throw DeleteException("El producto con el codigo de barra {" + codigo + "} no existe.");
    }

    std::string nombreTarget = productoEliminar->getNombre();

    this->raiz = eliminarRecursivo(this->raiz, nombreTarget, codigo);
}


/*Metodo utilizado para poder buscar por codigo de barra un producto (UTIL PARA ELIMINACION)*/

/*Al ordenarse por nombre no hay una trayectoria. Asi que se recorre por orden alfabetico de izquierda a derecha*/

Producto * ArbolAvl::buscarPorCodigobarra(NodoAvl* nodo, const std::string &codigo){
    if(nodo == nullptr){
        return nullptr;
    }

    if(nodo->getDato().getCodigoBarra() == codigo){
        return &(nodo->getDato());
    }

    Producto * izquierdo = buscarPorCodigobarra(nodo->getIzquierda(),codigo);

    if(izquierdo != nullptr){
        return izquierdo;
    }

    return buscarPorCodigobarra(nodo->getDerecha(),codigo);
}


/*Metodo utilizado para encontrar el nodo de mas bajo nivel de un subarbol para poder borrar un nodo (METODO AUXILIAR)*/
/*Este es el ultimo de los casos donde se elige la trayectoria para eliminar y balancear*/
NodoAvl* ArbolAvl::nodoMinimo(NodoAvl* nodo){

    NodoAvl * actual = nodo;

    while(actual->getIzquierda() != nullptr){
        actual = actual->getIzquierda();
    }

    return actual;
}

/*Metodo universal para poder eliminar un nodo recursivo. Este metodo es de los mas importantes ya que elimina
 y genera las rotaciones respectivas para mantenerlo balanceado*/

NodoAvl* ArbolAvl::eliminarRecursivo(NodoAvl* nodo, const std::string &nombre, const std::string& codigo){

    if(nodo == nullptr){
        return nodo;
    }


    if(nombre < nodo->getDato().getNombre()){
        nodo->setIzquierda(eliminarRecursivo(nodo->getIzquierda(),nombre,codigo));
    }
    else if(nombre > nodo->getDato().getNombre()){
        nodo->setDerecha(eliminarRecursivo(nodo->getDerecha(),nombre,codigo));
    }
    else{

        /*Caso en el que los nombres son iguales (EN ESTE CASO SIGO EL CAMINO QUE YO DECIDI PARA ELIMINAR LOS NOMBRES DERECHA)*/
        if(nodo->getDato().getCodigoBarra() != codigo){
            nodo->setDerecha(eliminarRecursivo(nodo->getDerecha(),nombre,codigo));
        }
        else{
            /*Casos en los que ya se encontro ( Un Hijo o es Hoja)*/
            if(nodo->getIzquierda() == nullptr || nodo->getDerecha() == nullptr){

                NodoAvl* temporal = nodo->getIzquierda() ? nodo->getIzquierda(): nodo->getDerecha();

                /*No tiene hijos*/
                if(temporal == nullptr){
                    delete nodo;
                    nodo = nullptr;
                }
                else{
                    /*Caso de un hijo*/

                    NodoAvl * nodoBorrar = nodo;
                    nodo = temporal;
                    delete nodoBorrar;
                }
            }
            else{/*Caso en donde tiene dos hijos (CRITICO)*/
                NodoAvl* sucesor = nodoMinimo(nodo->getDerecha());

                nodo->getDato() = sucesor->getDato();

                nodo->setDerecha(eliminarRecursivo(nodo->getDerecha(),sucesor->getDato().getNombre(),sucesor->getDato().getCodigoBarra()));
            }
        }
    }


    if(nodo == nullptr){
        return nullptr;
    }


    nodo->setAltura(1 + std::max(obtenerAltura(nodo->getIzquierda()), obtenerAltura(nodo->getDerecha())));

    int balance = obtenerFactorBalance(nodo);

    /*ROTACIONES PARA ELIMINAR*/

    /*Caso izquierda-izquierda*/
    if (balance > 1 && obtenerFactorBalance(nodo->getIzquierda()) >= 0) {
        return rotacionDerecha(nodo);
    }

    /*Caso izquireda-derecha*/
    if (balance > 1 && obtenerFactorBalance(nodo->getIzquierda()) < 0) {
        nodo->setIzquierda(rotacionIzquierda(nodo->getIzquierda()));
        return rotacionDerecha(nodo);
    }

    /*Caso derecha-derecha*/
    if (balance < -1 && obtenerFactorBalance(nodo->getDerecha()) <= 0) {
        return rotacionIzquierda(nodo);
    }

    /*Caso derecha-izquierda*/
    if (balance < -1 && obtenerFactorBalance(nodo->getDerecha()) > 0) {
        nodo->setDerecha(rotacionDerecha(nodo->getDerecha()));
        return rotacionIzquierda(nodo);
    }

    return nodo;
}

/*---****----Fin del Apartado de metodos utilizados para poder eliminar nodos del arbol---****---*/




