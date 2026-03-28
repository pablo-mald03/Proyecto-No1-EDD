#ifndef LISTAENLAZADA_H
#define LISTAENLAZADA_H

/*Includes de la clase*/
#include"nodolista.h"
#include"producto.h"

class ListaEnlazada
{

private:
    NodoLista * cabeza;
    NodoLista * cola;
    int longitud;

    //Metodo especial para hallar los nodos
    NodoLista* getNodo(int pos) const;

public:
    ListaEnlazada();
    ~ListaEnlazada();

    //Metodos para usar la lista
    void insertarAtras(const Producto&valor);
    void insertarFrente (const Producto&valor);

    void removerAtras();
    void removerFrente();

    bool esVacia() const;
    int getLongitud()const;

    void limpiar();

    //METODOS ESPECIALES QUE PERMITEN MOVERSE POR LA LISTA COMO SE DESEE
    void insertar(int pos, const Producto& valor);
    void eliminar(int pos);

    Producto& getValor(int pos);

    //Metodo para obtener la cabeza
    NodoLista* getCabeza() const;


};

#endif // LISTAENLAZADA_H
