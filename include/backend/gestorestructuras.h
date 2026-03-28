#ifndef GESTORESTRUCTURAS_H
#define GESTORESTRUCTURAS_H

//Includes de la clase
#include"listaenlazada.h"

class GestorEstructuras
{
public:
    GestorEstructuras();
    ~GestorEstructuras();

    /*Metodos para insertar datos en las estructuras provinientes del csv*/
    void insertarListas(std::string nombre, std::string key, std::string categoria, std::string fecha, std::string marca, double precio, int stock);

private:

    /*Atributos de la clase gestor*/
    ListaEnlazada *listaNoOrdenada = nullptr;
    ListaEnlazada * listaOrdenada = nullptr;

};

#endif // GESTORESTRUCTURAS_H
