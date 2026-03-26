#include "controlador.h"
#include<QElapsedTimer>


/*Clase delegada como el respectivo controlador de todo lo que se hara en el programa*/

Controlador::Controlador(QObject *parent)
    : QObject{parent}
{}


/*Metodo que permite comunicar a la ui para poder enviar el csv procesado*/
 /*
    * 1 -> AVL
    * 2 -> B
    * 3 -> B+
    * 4 -> LISTA
*/

void Controlador::procesarCsv(const std::vector<QString> & data){

    /*PENDIENTE (PROVISIONAL)*/

    QElapsedTimer timer;
    timer.start();

    for(const QString &linea: data){

        QStringList columnas = linea.split(",");


        std::string key = columnas[1].toStdString();

        std::string keyStr = columnas[0].toStdString();


        emit logArbolAvl("Insertado: " + QString::fromStdString(key), "green");

        // Simulación error
    }

    emit logArbolAvl("Error: clave negativa", "red");

    qint64 tiempo = timer.elapsed();

    emit tiempoProceso(1, tiempo);
}

/*Metodo que permite insertar los productos*/
void Controlador::insercionProducto(){

    emit logInsertArbolAvl("Error: clave negativa", "red");
}

/*Metodo que permite buscar los productos por nombre*/
void Controlador::buscarPorNombre(std::string nombre){

    /*Pendiente*/
    emit logBusquedaNombreArbolAvl("Nombre buscado: "+ QString::fromStdString(nombre), "green");
    emit logBusquedaNombreListaOrdenada("Nombre buscado: "+ QString::fromStdString(nombre), "green");
    emit logBusquedaNombreListaNoOrdenada("Nombre buscado: "+ QString::fromStdString(nombre), "green");
}


/*Metodo que permite buscar los productos por categoria*/
void Controlador::buscarPorCategoria(std::string categoria){

    /*Pendiente*/
    emit logBusquedaCategoriaArbolBMas("Categoria buscado: "+ QString::fromStdString(categoria), "green");
    emit logBusquedaCategoriaListaOrdenada("Categoria buscado: "+ QString::fromStdString(categoria), "green");
    emit logBusquedaCategoriaListaNoOrdenada("Categoria buscado: "+ QString::fromStdString(categoria), "green");
}

/*Metodo que permite buscar los productos por fechas*/
void Controlador::buscarPorFecha(std::string limiteInferior, std::string limiteSuperior){

    /*Pendiente*/
    emit logBusquedaFechasArbolB("Fecha buscado: "+ QString::fromStdString(limiteInferior), "green");
    emit logBusquedaFechasListaOrdenada("Fecha buscado: "+ QString::fromStdString(limiteInferior), "green");
    emit logBusquedaFechasListaNoOrdenada("Fecha buscado: "+ QString::fromStdString(limiteInferior), "green");
}
