#ifndef GESTORESTRUCTURAS_H
#define GESTORESTRUCTURAS_H

//Includes de la clase
#include "arbolavl.h"
#include "arbolb.h"
#include "erroreslectura.h"
#include"listaenlazada.h"
#include"producto.h"
#include <QStringList>
#include <QDate>

/*Arboles*/
//#include"arbolavl.h"

class GestorEstructuras
{
public:
    GestorEstructuras();
    ~GestorEstructuras();

    /*Metodo que permite insertar datos en el arbol AVL*/
    void insertarArbolAvl(std::string nombre, std::string key, std::string categoria, std::string fecha, std::string marca, double precio, int stock);


    /*Metodo que permite insertar datos en el arbol B*/
    void insertarArbolB(std::string nombre, std::string key, std::string categoria, std::string fecha, std::string marca, double precio, int stock);


    /*Metodos para insertar datos en las estructuras provinientes del csv*/
    void insertarListas(std::string nombre, std::string key, std::string categoria, std::string fecha, std::string marca, double precio, int stock);


    /*Metodo de validacion del csv delegado para validar*/
    void validarCsv(const QStringList& columnas, int fila);

    /*Metodo para agregar errores al log*/
    void agregarErrorLista(const std::string mensaje, int fila);

    /*Metodo que retorna el contenido del log de errores*/
    QString generarContenidoLog();

    /*Metodo que permite obtener la lista de errores*/
    ListaEnlazada<ErroresLectura>* getListaErrores();

    /*Metodo para saber si tiene errores la lista*/
    bool tieneErrores() const;

    /*Metodo utilizado para poder ordenar las listas acorde a los diferentes parametros*/
    void generarListaOrdenada(int criterio);

    /*Metodos getter y setter de la flag*/
    void setCargoArchivo(bool carga);
    bool getCargoArchivo();

    /*Metodo que retorna a la lista No ordenada*/
    ListaEnlazada<Producto> * getListaNoOrdenada();

    /*Metodo que permite exportar la lista al csv*/
    std::string serializarListaCsv();

    /*Metodo que permite obtener la raiz del arbol avl*/
    NodoAvl * getRaizArbolAvl();

    /*Metodo que permite obtener la raiz del arbol b*/
    NodoB * getRaizArbolB();

    /*Metodo que permite obtener el graphviz del arbol avl*/
    std::string obtenerGraphvizAvl();

private:

    /*Arbol AVL*/
    ArbolAvl * arbolAvl = nullptr;

    /*Arbol B*/
    ArbolB * arbolB = nullptr;

    /*Atributos de la clase gestor*/
    ListaEnlazada<Producto> *listaNoOrdenada = nullptr;
    ListaEnlazada<Producto> * listaOrdenada = nullptr;

    /*Lista de errores*/
    ListaEnlazada<ErroresLectura> * listaErrores = nullptr;

    /*Atributo flag de carga de csv para saber si guardar o refrescar*/
    bool cargoArchivo;

    /*Metodo delegado para poder validar la fecha*/
    bool esFechaISO(const QString& fecha);

    /*Metodo que permite verificar la integridad de la listaEnlazada*/
    bool existeProductoLista(const std::string &codigo);

};

#endif // GESTORESTRUCTURAS_H
