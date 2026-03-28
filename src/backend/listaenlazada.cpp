#include "listaenlazada.h"
#include <stdexcept>

/*Constructor de la lista enlazada*/
ListaEnlazada::ListaEnlazada(): cabeza(nullptr),cola(nullptr), longitud(0)
{

}

/*Destructor*/
ListaEnlazada::~ListaEnlazada()
{
    limpiar();
}

//Metodo que obtiene la cabeza de la lista enlazada para poder recorrerla sin mayor complejidad
NodoLista* ListaEnlazada::getCabeza() const {
    return this->cabeza;
}

//Metodo que retorna si la lista esta vacia
bool ListaEnlazada::esVacia() const{
    return this->cabeza == nullptr;
}

//Metodo que retorna la longitud de la lista
int ListaEnlazada::getLongitud() const
{
    return this->longitud;
}


//Metodo que agrega al frente de la lista
void ListaEnlazada::insertarAtras(const Producto& valor)
{
    NodoLista* nuevo = new NodoLista(valor);

    if (esVacia())
    {
        this->cabeza = cola = nuevo;
    }
    else
    {
        this->cola->setSiguiente(nuevo);
        nuevo->setAnterior(cola);
        this->cola = nuevo;
    }

    this->longitud++;
}

//Metodo para insertar al frente de la lista
void ListaEnlazada::insertarFrente(const Producto& valor){

    NodoLista* nuevo = new NodoLista(valor);

    if (esVacia())
    {
        this->cabeza = cola = nuevo;
    }
    else
    {
        nuevo->setSiguiente(cabeza);
        this->cabeza->setAnterior(nuevo);
        this->cabeza = nuevo;
    }

    this->longitud++;
}


//Metodo para remover atras de la lista
void ListaEnlazada::removerAtras()
{
    if (esVacia()){
        return;
    }

    NodoLista* temporal = this->cola;

    if (this->cabeza == this->cola)
    {
        this->cabeza = this->cola = nullptr;
    }
    else
    {
        this->cola = this->cola->getAnterior();
        this->cola->setSiguiente(nullptr);
    }

    delete temporal;
    this->longitud--;
}


//Metodo para remover al frente
void ListaEnlazada::removerFrente()
{
    if (esVacia()) return;

    NodoLista* temporal = this->cabeza;

    if (this->cabeza == cola)
    {
        this->cabeza = cola = nullptr;
    }
    else
    {
        this->cabeza = this->cabeza->getSiguiente();
        this->cabeza->setAnterior(nullptr);
    }

    delete temporal;
    this->longitud--;
}

//====================REGION DE METODOS ESPECIALES DE LA LISTA=================================

//Metodo privado de la clase (Busca nodos) por busqueda secuencial con modificacion O(n)
NodoLista* ListaEnlazada::getNodo(int indice) const{

    if(indice>=longitud){
        throw std::out_of_range("Indice fuera de rango de la lista enlazada");
    }

    NodoLista* actual;

    if (indice < this->longitud / 2)
    {
        actual = this->cabeza;
        for (int i = 0; i < indice; i++)
        {
            actual = actual->getSiguiente();
        }
    }
    else
    {
        actual = cola;
        for (int i = this->longitud - 1; i > indice; i--){
            actual = actual->getAnterior();
        }
    }
    return actual;
}

//Metodo que sirve para insertar en cualquier indice
void ListaEnlazada::insertar(int indice, const Producto& valor)
{
    if (indice > this->longitud){
        throw std::out_of_range("Indice fuera de rango de la lista enlazada");
    }

    if (indice == 0)
    {
        insertarFrente(valor);
        return;
    }

    if (indice == this->longitud)
    {
        insertarAtras(valor);
        return;
    }

    NodoLista* actual = getNodo(indice);
    NodoLista* anterior = actual->getAnterior();

    NodoLista* nuevo = new NodoLista(valor);

    nuevo->setSiguiente(actual);
    nuevo->setAnterior(anterior);

    anterior->setSiguiente(nuevo);
    actual->setAnterior(nuevo);

    this->longitud++;
}

//Metodo que sirve para eliminar un dato EN CUALQUIER POSICION
void ListaEnlazada::eliminar(int indice)
{
    if (indice >= this->longitud){
        throw std::out_of_range("Indice fuera de rango de la lista enlazada");
    }

    if (indice == 0)
    {
        removerFrente();
        return;
    }

    if (indice == this->longitud - 1)
    {
        removerAtras();
        return;
    }

    NodoLista* actual = getNodo(indice);
    NodoLista* anterior = actual->getAnterior();
    NodoLista* siguiente = actual->getSiguiente();

    anterior->setSiguiente(siguiente);
    siguiente->setAnterior(anterior);

    delete actual;
    this->longitud--;
}

//Metodo que permite obtener un valor por posicion
Producto& ListaEnlazada::getValor(int indice)
{
    return getNodo(indice)->getDato();
}

//====================FIN DE LA REGION DE METODOS ESPECIALES DE LA LISTA=================================

/*Metodo que permite limpiar la lista por completo*/
void ListaEnlazada::limpiar()
{
    while (!esVacia())
        removerFrente();
}
