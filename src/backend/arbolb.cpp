#include "arbolb.h"
#include "insertexception.h"

/*Constructor del arbol se deja el grado minimo del arbol a eleccion*/
ArbolB::ArbolB(int _t):
    raiz(nullptr),
    t(_t)
{

}

/*Destructor*/
ArbolB::~ArbolB(){

}


/*Metodo que permite insertar un nuevo producto en un arbol. Maneja la integridad*/
void ArbolB::insertar(const Producto &producto){

    if (existeCodigo(this->raiz, producto.getCodigoBarra())) {
        throw InsertException("El codigo de barras { " +
                              producto.getCodigoBarra() + " } ya existe para el producto: " +
                              producto.getNombre() + " [ARBOL B]");
    }

    /*Insercion normal*/
    this->insertarNuevo(producto);
}


/*Metodo que permite dividir a los hijos del arbol para ir generando la funcionalidad de ir prestando hijos*/
void ArbolB::dividirHijo(NodoB* padre, int i, NodoB* lleno){

    /*Nuevo nodo que recibe la mitad*/
    NodoB* nuevo = new NodoB(lleno->getEsHoja());

    /*Se pasan las ultimas claves al nuevo nodo*/
    for (int j = 0; j < this->t - 1; j++) {
        Producto p = lleno->getClaves().getValor(this->t);
        nuevo->getClaves().insertarAtras(p);
        lleno->getClaves().eliminar(this->t);
    }

    /*Si no es hoja tambien se mueven los hijos*/
    if (!lleno->getEsHoja()) {
        for (int j = 0; j < this->t; j++) {
            NodoB* h = lleno->getHijos().getValor(this->t);
            nuevo->getHijos().insertarAtras(h);
            lleno->getHijos().eliminar(this->t);
        }
    }


    /*Se inserta el nuevo hermano en la lista de hijos del padre*/
    padre->getHijos().insertar(i + 1, nuevo);


    /*Se sube la clave que quedo en el centro (Hacia el padre)*/
    Producto mediana = lleno->getClaves().getValor(t - 1);
    padre->getClaves().insertar(i, mediana);
    lleno->getClaves().eliminar(t - 1);
}

/*Metodo que permite insertar en la lista de claves que tienen los nodos del arbol*/
/*
*  Se busca de derecha a izquierda (para mantener orden)
*/
void ArbolB::insertarNoLleno(NodoB* nodo, const Producto &producto){
    int i = nodo->getClaves().getLongitud() - 1;

    /*Evaluacion si el nodo es hoja*/
    if(nodo->getEsHoja()){

        /*Es la simple evaluacion para saber donde se ordena por menor fecha*/
        while (i >= 0) {

            Producto actual = nodo->getClaves().getValor(i);

            if (producto.getFechaExpiracion() < actual.getFechaExpiracion() ||
                (producto.getFechaExpiracion() == actual.getFechaExpiracion() && producto.getCodigoBarra() < actual.getCodigoBarra())) {
                i--;
            } else {
                break;
            }
        }

        nodo->getClaves().insertar(i + 1, producto);
    }
    else{
        /*Caso en el que el nodo no es hoja*/


        /*Caso de busqueda por cual nodo bajar*/
        while (i >= 0) {

            Producto actual = nodo->getClaves().getValor(i);
            if (producto.getFechaExpiracion() < actual.getFechaExpiracion() ||
                (producto.getFechaExpiracion() == actual.getFechaExpiracion() && producto.getCodigoBarra() < actual.getCodigoBarra())) {
                i--;
            } else {
                break;
            }
        }

        i++; //Este es el indice ya del hijito

        /*Se evalua si el hijo esta lleno se divide primero*/

        if (nodo->getHijos().getValor(i)->getClaves().getLongitud() == (2 * (this->t) - 1)) {

            dividirHijo(nodo, i, nodo->getHijos().getValor(i));

            Producto medianaSubida = nodo->getClaves().getValor(i);

            if (producto.getFechaExpiracion() > medianaSubida.getFechaExpiracion() ||
                (producto.getFechaExpiracion() == medianaSubida.getFechaExpiracion() && producto.getCodigoBarra() > medianaSubida.getCodigoBarra())) {
                i++;
            }
        }

        insertarNoLleno(nodo->getHijos().getValor(i), producto);
    }
}



/*Metodo que permite insertar datos al arbol B*/
void ArbolB::insertarNuevo(const Producto &producto){

    if(this->raiz == nullptr){
        this->raiz = new NodoB(true);
        this->raiz->getClaves().insertarAtras(producto);
        return;
    }

    /*Verificacion si la lista esta llena (supero el limite de claves)*/
    if (raiz->getClaves().getLongitud() == (2 * t - 1)) {

        NodoB* nuevaRaiz = new NodoB(false);
        nuevaRaiz->getHijos().insertarAtras(this->raiz);

        /*Se divide la raiz y se busca hacia donde hacer el merge*/
        dividirHijo(nuevaRaiz, 0, this->raiz);

        int i = 0;

        Producto mediana = nuevaRaiz->getClaves().getValor(0);

        /*Mi parametro es la fecha y la primary key*/
        if (producto.getFechaExpiracion() > mediana.getFechaExpiracion() ||
            (producto.getFechaExpiracion() == mediana.getFechaExpiracion() && producto.getCodigoBarra() > mediana.getCodigoBarra())) {
            i++;
        }

        /*Se actualiza la raiz del arbol*/
        insertarNoLleno(nuevaRaiz->getHijos().getValor(i), producto);
        this->raiz = nuevaRaiz;
    }
    else{
        /*Si no esta lleno simplemente se inserta*/
        insertarNoLleno(this->raiz, producto);
    }
}


/*----***---Metodos de busqueda---***----*/

/*Metodo que permite validar la integridad*/

bool ArbolB::existeCodigo(NodoB* nodo, const std::string &codigo){
    if(nodo == nullptr){
        return false;
    }

    /*Se revisan las claves del nodo raiz actual*/
    for (int i = 0; i < nodo->getClaves().getLongitud(); i++) {
        if (nodo->getClaves().getValor(i).getCodigoBarra() == codigo) {
            return true;
        }
    }

    /*Se busca en los hijos de forma recursiva*/
    if (!nodo->getEsHoja()) {
        for (int i = 0; i < nodo->getHijos().getLongitud(); i++) {
            if (existeCodigo(nodo->getHijos().getValor(i), codigo)) {
                return true;
            }
        }
    }

    return false;
}


/*----***---Metodos de busqueda---***----*/

