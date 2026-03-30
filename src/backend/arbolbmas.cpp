#include "arbolbmas.h"
#include "insertexception.h"

/*Constructor del arbol B+*/
ArbolBMas::ArbolBMas(int _orden):
    orden(_orden),
    raiz(nullptr)
{

}

/*Destructor*/
ArbolBMas::~ArbolBMas(){
    if(raiz != nullptr){
        delete this->raiz;
    }
}

/*Metodo que permite obtener la raiz del arbol*/
NodoBMas* ArbolBMas::getRaiz(){
    return this->raiz;
}

/*Metodo que permite buscar por categoria (lista de productos por categoria)*/
/*
* La estrategia utilizada es transportarse por las conexiones que este arbol genera hacia los hijos
* de esta forma se puede bajar demasiado rapido sin recursividad y empezar a subir hasta toparse con algo que no esta
* ordenado alfabeticamente y rompe
*/
ListaEnlazada<Producto>  ArbolBMas::buscarPorCategoria(const std::string &categoriaBuscada){
    ListaEnlazada<Producto> resultados;

    if (this->raiz == nullptr) {
        return resultados;
    }

    NodoBMas* actual = this->raiz;

    /*Fase 1: Busqueda vertical. Directamente se baja por las rutas conectadas*/

    while (!actual->getEsHoja()) {
        int i = 0;
        int nClave = actual->getClaves().getLongitud();

        while (i < nClave) {
            Producto pActual = actual->getClaves().getValor(i);

            /*Se busca solo por la categoria para poder listar*/
            if (categoriaBuscada < pActual.getCategoria()) {
                break;
            }

            i++;
        }

        actual = actual->getHijos().getValor(i);
    }

    /*Fase 2: recorrido horizontal. Se va buscando la hoja donde podria estar la categoria*/
    bool continuarBuscando = true;

    while (actual != nullptr && continuarBuscando) {
        int nClave = actual->getClaves().getLongitud();

        for (int i = 0; i < nClave; i++) {
            Producto pActual = actual->getClaves().getValor(i);

            if (pActual.getCategoria() == categoriaBuscada) {
                resultados.insertar(resultados.getLongitud(), pActual);
            }
            else if (pActual.getCategoria() > categoriaBuscada) {

                /*Ya que los datos estan ordenados por categoria alfabeticamente se termina*/

                continuarBuscando = false;
                break;
            }
        }

        if (continuarBuscando) {
            actual = actual->getSiguiente();
        }
    }

    return resultados;
}

/*Metodo que permite insertar un producto*/
void ArbolBMas::insertar(const Producto &producto){

    if (yaExiste(this->raiz, producto)) {
        throw InsertException("El codigo de barras { " +
                              producto.getCodigoBarra() + " } ya existe para el producto: " +
                              producto.getNombre() + " [ARBOL B+]");
    }

    /*Caso base arbol vacio*/
    if(raiz == nullptr){
        this->raiz = new NodoBMas(true);
        insertarEnNodo(raiz, producto);
        return;
    }

    /*Validacion si la raiz del arbol esta llena este crece de altura*/
    if (this->raiz->getClaves().getLongitud() >= orden) {

        NodoBMas* nuevaRaiz = new NodoBMas(false);

        /*Raiz mas antigua es el primer hijo*/
        nuevaRaiz->getHijos().insertar(0, this->raiz);

        if (this->raiz->getEsHoja()) {
            dividirHoja(this->raiz, nuevaRaiz, 0);
        } else {
            dividirInterno(this->raiz, nuevaRaiz, 0);
        }

        this->raiz = nuevaRaiz;

    }

    insertarRecursivo(this->raiz, nullptr, producto, -1);
}

/*--****----Apartado de metodos que permiten insertar en el arbol B+---****--*/

/*Metodo que permite insertar datos en un nodo del arbol*/
/*
* Para el caso de este arbol lo que pasa es que inserta en las hojas y conserva referencias arriba es decir genera duplicidad
* pero es mas apto para poder almacenar datos y remover datos
*/
void ArbolBMas::insertarEnNodo(NodoBMas* nodo, const Producto &producto){

    int i = 0;

    int nClave = nodo->getClaves().getLongitud();

    while (i < nClave) {
        Producto actual = nodo->getClaves().getValor(i);

        if (producto.getCategoria() < actual.getCategoria()) {
            break;
        }

        if (producto.getCategoria() == actual.getCategoria() &&
            producto.getCodigoBarra() < actual.getCodigoBarra()){
            break;
        }

        i++;

    }

    nodo->getClaves().insertar(i, producto);
}

/*Metodo que permite generar la recursividad para poder insertar en el lugar correcto*/
void ArbolBMas::insertarRecursivo(NodoBMas *nodo, NodoBMas *padre, Producto producto, int indiceEnPadre) {

    /*Si es hoja simplemente manda a insertar dentro de la lista*/
    if (nodo->getEsHoja()) {
        insertarEnNodo(nodo, producto);
    } else {

        /*Sino busca por que hijo bajar*/
        int i = 0;
        int nClave = nodo->getClaves().getLongitud();

        while (i < nClave) {
            Producto actual = nodo->getClaves().getValor(i);
            if (producto.getCategoria() < actual.getCategoria()) {
                break;
            }

            if (producto.getCategoria() == actual.getCategoria() &&
                producto.getCodigoBarra() < actual.getCodigoBarra()) {
                break;
            }
            i++;
        }

        NodoBMas* hijo = nodo->getHijos().getValor(i);

        /*Division si el hijo esta lleno se divide antes de entrar*/
        /*Fue como yo lo pense para evitar andar haciendo tanto merge descontrolado*/
        if (hijo->getClaves().getLongitud() >= orden) {
            if (hijo->getEsHoja()) {
                dividirHoja(hijo, nodo, i);
            } else {
                dividirInterno(hijo, nodo, i);
            }

            // Re-evaluar por cuál hijo bajar tras la división
            Producto claveSubida = nodo->getClaves().getValor(i);
            if (producto.getCategoria() > claveSubida.getCategoria() ||
                (producto.getCategoria() == claveSubida.getCategoria() &&
                 producto.getCodigoBarra() >= claveSubida.getCodigoBarra())) {
                hijo = nodo->getHijos().getValor(i + 1);
            }
        }

        insertarRecursivo(hijo, nodo, producto, i);
    }
}


/*Metodo que permite generar la division de las hojas cuando se sobrepasa el limite*/
void ArbolBMas::dividirHoja(NodoBMas* hoja, NodoBMas* padre, int indiceEnPadre) {

    NodoBMas* nuevaHoja = new NodoBMas(true);
    int mitad = orden / 2;

    /*Se conecta la nueva hoja a la lista enlazada*/
    nuevaHoja->setSiguiente(hoja->getSiguiente());
    hoja->setSiguiente(nuevaHoja);

    /*Se mueve la mitad derecha de productos a la nueva hoja*/
    while (hoja->getClaves().getLongitud() > mitad) {

        Producto product = hoja->getClaves().getValor(mitad);

        nuevaHoja->getClaves().insertar(nuevaHoja->getClaves().getLongitud(), product);

        hoja->getClaves().eliminar(mitad);
    }

    /*El primer elemento de la nueva hoja sube al padre es decir que se copia*/
    padre->getClaves().insertar(indiceEnPadre, nuevaHoja->getClaves().getValor(0));
    padre->getHijos().insertar(indiceEnPadre + 1, nuevaHoja);
}

/*Metodo que permite dividir las claves a la mitad para poder generar la integracion*/

void ArbolBMas::dividirInterno(NodoBMas* interno, NodoBMas* padre, int indiceEnPadre) {
    NodoBMas* nuevoInterno = new NodoBMas(false);

    /*Se obteniene la mitad de la lista de claves*/
    int mitad = orden / 2;

    Producto subido = interno->getClaves().getValor(mitad);
    interno->getClaves().eliminar(mitad);


    /*Se mueven las llaves restantes al nuevo nodo*/
    while (interno->getClaves().getLongitud() > mitad) {
        Producto product = interno->getClaves().getValor(mitad);
        nuevoInterno->getClaves().insertar(nuevoInterno->getClaves().getLongitud(), product);
        interno->getClaves().eliminar(mitad);
    }

    /*Se mueven a los hijos al nuevo nodo*/
    /*Se genera una limpieza en la parte derehca del nodo y se le pasa la carga al hermano*/
    while (interno->getHijos().getLongitud() > mitad + 1) {
        NodoBMas* hijo = interno->getHijos().getValor(mitad + 1);
        nuevoInterno->getHijos().insertar(nuevoInterno->getHijos().getLongitud(), hijo);
        interno->getHijos().eliminar(mitad + 1);
    }

    padre->getClaves().insertar(indiceEnPadre, subido);
    padre->getHijos().insertar(indiceEnPadre + 1, nuevoInterno);
}

/*Metodo que permite conservar la integridad del arbol ya que hace que el codigo de barras sea unico*/
/*
*La tactica de busqueda es simple ya que se aprovecha la caracteristica de este arbol de ser super rapido
*de esta forma se permite lograr buscar super rapido gracias al nodo siguiente que interconecta
*
*/
bool ArbolBMas::yaExiste(NodoBMas* nodoRaiz, const Producto &producto){

    if (nodoRaiz == nullptr) {
        return false;
    }

    NodoBMas* actual = nodoRaiz;

    /*Se baja en linea recta hasta llegar a la primera hoja (EFICIENCIA)*/
    while (!actual->getEsHoja()) {
        actual = actual->getHijos().getValor(0);
    }

    /*Si no esta directamente en una hoja se tiene que seguir recorriendo el arbol buscando una clave*/
    /*
    * La tactica esta en que se siguen recorriendo las hojas usando la lista enlazada conectada
    */
    while (actual != nullptr) {
        int nClaves = actual->getClaves().getLongitud();

        for (int i = 0; i < nClaves; i++) {
            if (actual->getClaves().getValor(i).getCodigoBarra() == producto.getCodigoBarra()) {
                return true;
            }
        }

        // Se salta a la siguiente hoja esto es lo que lo hace super rapido
        actual = actual->getSiguiente();
    }

    return false;
}

/*--****----Fin del Apartado de metodos que permiten insertar en el arbol B+---****--*/
