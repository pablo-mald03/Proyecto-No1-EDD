#ifndef NODOLISTA_H
#define NODOLISTA_H

template<typename T>
class NodoLista
{

private:
    T dato;
    NodoLista<T> * siguiente;
    NodoLista<T> * anterior;


public:
    NodoLista(const T &producto);
    ~NodoLista();

    /*Metodos getters y setters*/
    T& getDato();

    NodoLista<T>* getSiguiente() const;
    NodoLista<T>* getAnterior() const;

    void setSiguiente(NodoLista<T>* _siguiente);
    void setAnterior(NodoLista<T>* _anterior);

};

#endif // NODOLISTA_H
