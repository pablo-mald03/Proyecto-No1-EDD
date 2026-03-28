#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <QObject>

/*Includes de backend*/
#include"gestorestructuras.h"

class Controlador : public QObject
{
    Q_OBJECT
public:
    explicit Controlador(QObject *parent = nullptr);
    ~Controlador();

private:
    GestorEstructuras * gestorBackend = nullptr;

public slots:
    void procesarCsv(const std::vector<QString> & data);

    /*Metodos que permiten realizar el CRUD de la aplicacion*/
    void insercionProducto();
    void buscarPorNombre(std::string nombre);
    void buscarPorCategoria(std::string categoria);
    void buscarPorFecha(std::string limiteInferior, std::string limiteSuperior);
    void eliminarProducto(std::string codigo);
    void listarProductos();


    /*Metodos que permiten obtener las referencias de los arboles*/
    void obtenerArbolAvl();
    void obtenerArbolB();
    void obtenerArbolBMas();

signals:

    /*Metodos que permiten enviar la informacion para poder graficar los arboles*/
    void enviarArbolAvl(int * arbol);
    void enviarArbolB(int * arbol);
    void enviarArbolBMas(int * arbol);


    /*Signals que permiten comunicarse con la pantalla del csv */
    void logArbolAvl(QString mensaje, QString color);
    void logArboB(QString mensaje, QString color);
    void logArbolBMas(QString mensaje, QString color);
    void logLista(QString mensaje, QString color);

    /*Signal que permite setear el tiempo en el label*/
    void tiempoProceso(int estructura, qint64 milisegundos);

    /*Fin de los Signals que permiten comunicarse con la pantalla del csv */


    /*Signals que permiten comunicarse con la pantalla de agregar */
    void logInsertArbolAvl(QString mensaje, QString color);
    void logInsertArbolB(QString mensaje, QString color);
    void logInsertArbolBMas(QString mensaje, QString color);
    void logInsertListaOrdenada(QString mensaje, QString color);
    void logInsertListaNoOrdenada(QString mensaje, QString color);

    /*Signal que permite setear el tiempo en el label*/
    void tiempoProcesoInsert (int estructura, qint64 milisegundos);

    /*Fin de las Signals que permiten comunicarse con la pantalla de agregar */


    /*Signals que permiten comunicarse con la pantalla de buscar por nombre */
    void logBusquedaNombreArbolAvl(QString mensaje, QString color);
    void logBusquedaNombreListaOrdenada(QString mensaje, QString color);
    void logBusquedaNombreListaNoOrdenada(QString mensaje, QString color);

    /*Signal que permite setear el tiempo en el label*/
    void tiempoProcesoBusquedaNombre (int estructura, qint64 milisegundos);

    /*Fin de las Signals que permiten comunicarse con la pantalla de buscar por nombrer */

    /*Signals que permiten comunicarse con la pantalla de buscar por nombre */
    void logBusquedaCategoriaArbolBMas(QString mensaje, QString color);
    void logBusquedaCategoriaListaOrdenada(QString mensaje, QString color);
    void logBusquedaCategoriaListaNoOrdenada(QString mensaje, QString color);

    /*Signal que permite setear el tiempo en el label*/
    void tiempoProcesoBusquedaCategoria (int estructura, qint64 milisegundos);

    /*Fin de las Signals que permiten comunicarse con la pantalla de buscar por nombre*/


    /*Signals que permiten comunicarse con la pantalla de buscar por rango de fechas */
    void logBusquedaFechasArbolB(QString mensaje, QString color);
    void logBusquedaFechasListaOrdenada(QString mensaje, QString color);
    void logBusquedaFechasListaNoOrdenada(QString mensaje, QString color);

    /*Signal que permite setear el tiempo en el label*/
    void tiempoProcesoBusquedaFechas (int estructura, qint64 milisegundos);

    /*Fin de las Signals que permiten comunicarse con la pantalla de buscar por nombre */



    /*Signals que permiten comunicarse con la pantalla de eliminar productos */
    void logEliminarArbolAvl(QString mensaje, QString color);
    void logEliminarArbolB(QString mensaje, QString color);
    void logEliminarArbolBMas(QString mensaje, QString color);
    void logEliminarListaOrdenada(QString mensaje, QString color);
    void logEliminarListaNoOrdenada(QString mensaje, QString color);

    /*Signal que permite setear el tiempo en el label*/
    void tiempoEliminarProceso(int estructura, qint64 milisegundos);

    /*Fin de los Signals que permiten comunicarse con la pantalla de listar productos */

    /*Signals que permiten comunicarse con la pantalla de buscar por nombre */
    void logListarArbolAvl(QString mensaje, QString color);
    void logListarListaOrdenada(QString mensaje, QString color);
    void logListarListaNoOrdenada(QString mensaje, QString color);

    /*Signal que permite setear el tiempo en el label*/
    void tiempoProcesoListarNombre (int estructura, qint64 milisegundos);

    /*Fin de las Signals que permiten comunicarse con la pantalla de listai productos */

};

#endif // CONTROLADOR_H
