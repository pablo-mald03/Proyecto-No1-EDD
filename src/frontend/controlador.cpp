#include "controlador.h"
#include "insertexception.h"
#include "readercsvexception.h"
#include<QElapsedTimer>


/*Clase delegada como el respectivo controlador de todo lo que se hara en el programa*/

Controlador::Controlador(QObject *parent)
    : QObject{parent}, gestorBackend(new GestorEstructuras())
{
}

/*Destructor*/
Controlador::~Controlador(){
    if(this->gestorBackend != nullptr){
        delete this->gestorBackend;
        this->gestorBackend = nullptr;
    }
}


/*---METODO QUE ELIMINA POR COMPLETO TODO EL BACKEND CUANDO SE EXPORTA EL CSV---*/
void Controlador::limpiarDatos(){

    if(this->gestorBackend != nullptr){
        delete this->gestorBackend;
        this->gestorBackend = nullptr;
    }

    this->gestorBackend = new GestorEstructuras();

    this->verificarErrores();
    this->evaluarEstadoCerrarCsv();

}


/*Signal que permite retornar el csv para poderlo guardar*/
void Controlador::guardarArchivoCsv(){

    if (this->gestorBackend == nullptr){
        return;
    }

    std::string contenidoRaw = this->gestorBackend->serializarListaCsv();

    emit contenidoCsvListo(QString::fromStdString(contenidoRaw));
}


/*Metodo que permite rellenar los datos actualizados con los de la lista*/
void Controlador::actualizarDatosIngresados(){

    if (this->gestorBackend == nullptr) return;

    NodoLista<Producto>* actual = this->gestorBackend->getListaNoOrdenada()->getCabeza();

    while (actual != nullptr) {
        const Producto& p = actual->getDato();

        QString lineaCsv = QString("\"%1\",\"%2\",\"%3\",\"%4\",\"%5\",%6,%7")
                               .arg(QString::fromStdString(p.getNombre()),
                                    QString::fromStdString(p.getCodigoBarra()),
                                    QString::fromStdString(p.getCategoria()),
                                    QString::fromStdString(p.getFechaExpiracion()),
                                    QString::fromStdString(p.getMarca()))
                               .arg(p.getPrecio())
                               .arg(p.getStock());

        emit logCargaCsv(lineaCsv, "white");
        actual = actual->getSiguiente();
    }

    emit logCargaCsv("Vista sincronizada con la lista enlazada.", "green");
}


/*Metodo que permite comunicar a la ui para poder enviar el csv procesado*/
 /*
    * 1 -> AVL
    * 2 -> B
    * 3 -> B+
    * 4 -> LISTA
*/
void Controlador::procesarCsv(const std::vector<QString> & data){

    if(this->gestorBackend != nullptr){
        delete this->gestorBackend;
        this->gestorBackend = nullptr;
    }

    this->gestorBackend = new GestorEstructuras();


    if (data.empty()) {
        this->gestorBackend->agregarErrorLista("El archivo de entrada esta vacio", 0);
        emit logCargaCsv("El archivo esta vacio.", "red");
        return;
    }

    int filaActual = 1;

    /*Buffer filtrado*/
    std::vector<QString> datosValidados;

    for (const QString &linea : data) {

        if (linea.trimmed().isEmpty()) {
            filaActual++;
            continue;
        }

        try {
            QStringList columnas = linea.split(",");

            this->gestorBackend->validarCsv(columnas, filaActual);

            datosValidados.push_back(linea);

            emit logCargaCsv(linea, "white");

        }catch (const ReaderCsvException& e) {
            emit logCargaCsv(QString::fromUtf8(e.what()), "red");
        }
        catch (const std::exception& e) {
            emit logCargaCsv("Error inesperado: " + QString::fromStdString(e.what()), "red");
        }
        filaActual++;
    }

    if (!datosValidados.empty()) {

        this->insertarListaCsv(datosValidados);
        this->insertarArbolAvlCsv(datosValidados);
        this->insertarArbolBCsv(datosValidados);
        this->insertarArbolBMasCsv(datosValidados);

        emit logCargaCsv("Proceso finalizado. Filas a insertar: " + QString::number(datosValidados.size()), "green");

        /*Orden base*/
        this->gestorBackend->generarListaOrdenada(1);

        /*Set de carga*/
        this->gestorBackend->setCargoArchivo(true);

        this->verificarErrores();

        this->evaluarEstadoCerrarCsv();

    }
    else {
        emit logCargaCsv("No se inserto nada: Todas las lineas tenian errores.", "yellow");
    }
}


/*Metodo utilizado para insertar los datos dentro del arbol AVL*/
void Controlador::insertarArbolAvlCsv(const std::vector<QString> & data){

    QElapsedTimer timer;
    timer.start();

    int filaActual = 1;

    for(const QString &linea: data){

        QStringList columnas = linea.split(",");

        if (columnas.size() < 7) {
            this->gestorBackend->agregarErrorLista("Fila con columnas insuficientes", filaActual);
            emit logArbolAvl("Error: Fila malformada en linea " + QString::number(filaActual), "red");
            filaActual++;
            continue;
        }

        std::string nombre = columnas[0].toStdString();
        std::string key = columnas[1].toStdString();
        std::string categoria = columnas[2].toStdString();
        std::string fechaExp = columnas[3].toStdString();
        std::string marca = columnas[4].toStdString();

        bool okPrecio;
        bool okStock;

        double precio = columnas[5].toDouble(&okPrecio);
        int stock = columnas[6].toInt(&okStock);

        if (!okPrecio || !okStock) {
            emit logArbolAvl("Error en conversion numerica: " + linea, "red");
            continue;
        }
        try{

            this->gestorBackend->insertarArbolAvl(nombre,key,categoria,fechaExp,marca,precio,stock);
            emit logArbolAvl("Insertado: " + QString::fromStdString(key), "green");
        }
        catch (const InsertException& e) {
            this->gestorBackend->agregarErrorLista(e.what(), filaActual);
            emit logArbolAvl("Fila: " + QString::number(filaActual) + " " + QString::fromStdString(e.what()), "red");
        }
        catch (const ReaderCsvException& e) {

            this->gestorBackend->agregarErrorLista(e.what(), filaActual);
            emit logArbolAvl("Fila: " + QString::number(filaActual) + " " + QString::fromStdString(e.what()), "red");
        }
        catch (const std::exception& ex) {
            emit logArbolAvl("Error inesperado: " + QString::fromStdString(ex.what()) + ". Fila: " + QString::number(filaActual) , "red");
        }

        filaActual++;
    }

    qint64 tiempo = timer.elapsed();
    emit tiempoProceso(1, tiempo);
}



/*Metodo utilizado para insertar los datos dentro del arbol B*/
void Controlador::insertarArbolBCsv(const std::vector<QString> & data){

    QElapsedTimer timer;
    timer.start();

    int filaActual = 1;

    for(const QString &linea: data){

        QStringList columnas = linea.split(",");

        if (columnas.size() < 7) {
            this->gestorBackend->agregarErrorLista("Fila con columnas insuficientes", filaActual);
            emit logArbolB("Error: Fila malformada en linea " + QString::number(filaActual), "red");
            filaActual++;
            continue;
        }

        std::string nombre = columnas[0].toStdString();
        std::string key = columnas[1].toStdString();
        std::string categoria = columnas[2].toStdString();
        std::string fechaExp = columnas[3].toStdString();
        std::string marca = columnas[4].toStdString();

        bool okPrecio;
        bool okStock;

        double precio = columnas[5].toDouble(&okPrecio);
        int stock = columnas[6].toInt(&okStock);

        if (!okPrecio || !okStock) {
            emit logArbolB("Error en conversion numerica: " + linea, "red");
            continue;
        }
        try{

            this->gestorBackend->insertarArbolB(nombre,key,categoria,fechaExp,marca,precio,stock);
            emit logArbolB("Insertado: " + QString::fromStdString(key), "green");
        }
        catch (const InsertException& e) {
            this->gestorBackend->agregarErrorLista(e.what(), filaActual);
            emit logArbolB("Fila: " + QString::number(filaActual) + " " + QString::fromStdString(e.what()), "red");
        }
        catch (const ReaderCsvException& e) {

            this->gestorBackend->agregarErrorLista(e.what(), filaActual);
            emit logArbolB("Fila: " + QString::number(filaActual) + " " + QString::fromStdString(e.what()), "red");
        }
        catch (const std::exception& ex) {
            emit logArbolB("Error inesperado: " + QString::fromStdString(ex.what()) + ". Fila: " + QString::number(filaActual) , "red");
        }

        filaActual++;
    }

    qint64 tiempo = timer.elapsed();
    emit tiempoProceso(2, tiempo);
}


/*Metodo utilizado para insertar los datos dentro del arbol B+*/
void Controlador::insertarArbolBMasCsv(const std::vector<QString> & data){

    QElapsedTimer timer;
    timer.start();

    int filaActual = 1;

    for(const QString &linea: data){

        QStringList columnas = linea.split(",");

        if (columnas.size() < 7) {
            this->gestorBackend->agregarErrorLista("Fila con columnas insuficientes", filaActual);
            emit logArbolBMas("Error: Fila malformada en linea " + QString::number(filaActual), "red");
            filaActual++;
            continue;
        }

        std::string nombre = columnas[0].toStdString();
        std::string key = columnas[1].toStdString();
        std::string categoria = columnas[2].toStdString();
        std::string fechaExp = columnas[3].toStdString();
        std::string marca = columnas[4].toStdString();

        bool okPrecio;
        bool okStock;

        double precio = columnas[5].toDouble(&okPrecio);
        int stock = columnas[6].toInt(&okStock);

        if (!okPrecio || !okStock) {
            emit logArbolBMas("Error en conversion numerica: " + linea, "red");
            continue;
        }
        try{

            this->gestorBackend->insertarArbolBMas(nombre,key,categoria,fechaExp,marca,precio,stock);
            emit logArbolBMas("Insertado: " + QString::fromStdString(key), "green");
        }
        catch (const InsertException& e) {
            this->gestorBackend->agregarErrorLista(e.what(), filaActual);
            emit logArbolBMas("Fila: " + QString::number(filaActual) + " " + QString::fromStdString(e.what()), "red");
        }
        catch (const ReaderCsvException& e) {

            this->gestorBackend->agregarErrorLista(e.what(), filaActual);
            emit logArbolBMas("Fila: " + QString::number(filaActual) + " " + QString::fromStdString(e.what()), "red");
        }
        catch (const std::exception& ex) {
            emit logArbolBMas("Error inesperado: " + QString::fromStdString(ex.what()) + ". Fila: " + QString::number(filaActual) , "red");
        }

        filaActual++;
    }

    qint64 tiempo = timer.elapsed();
    emit tiempoProceso(3, tiempo);
}


/*Metodo que permite llenar las listas con el arreglo de entrada del csv*/
void Controlador::insertarListaCsv(const std::vector<QString> & data){

    QElapsedTimer timer;
    timer.start();

    int filaActual = 1;

    for(const QString &linea: data){

        QStringList columnas = linea.split(",");

        if (columnas.size() < 7) {
            this->gestorBackend->agregarErrorLista("Fila con columnas insuficientes", filaActual);
            emit logLista("Error: Fila malformada en linea " + QString::number(filaActual), "red");
            filaActual++;
            continue;
        }

        std::string nombre = columnas[0].toStdString();
        std::string key = columnas[1].toStdString();
        std::string categoria = columnas[2].toStdString();
        std::string fechaExp = columnas[3].toStdString();
        std::string marca = columnas[4].toStdString();

        bool okPrecio;
        bool okStock;

        double precio = columnas[5].toDouble(&okPrecio);
        int stock = columnas[6].toInt(&okStock);

        if (!okPrecio || !okStock) {
            emit logLista("Error en conversion numerica: " + linea, "red");
            continue;
        }
        try{

            this->gestorBackend->insertarListas(nombre,key,categoria,fechaExp,marca,precio,stock);

            emit logLista("Insertado: " + QString::fromStdString(key), "green");

        }catch (const ReaderCsvException& e) {

            this->gestorBackend->agregarErrorLista(e.what(), filaActual);
            emit logLista("Fila: " + QString::number(filaActual) + " " + QString::fromStdString(e.what()), "red");
        }
        catch (const std::exception& ex) {
            emit logLista("Error inesperado: " + QString::fromStdString(ex.what()) + ". Fila: " + QString::number(filaActual) , "red");
        }

        filaActual++;
    }

    qint64 tiempo = timer.elapsed();
    emit tiempoProceso(4, tiempo);
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

/*Metodo que permite eliminar un producto por codigo*/
void Controlador::eliminarProducto(std::string codigo){

    /*Pendiente*/
    emit logEliminarArbolAvl("Producto eliminado: "+ QString::fromStdString(codigo), "green");
    emit logEliminarArbolB("Producto eliminado: "+ QString::fromStdString(codigo), "green");
    emit logEliminarArbolBMas("Producto eliminado: "+ QString::fromStdString(codigo), "green");
    emit logEliminarListaOrdenada("Producto eliminado: "+ QString::fromStdString(codigo), "green");
    emit logEliminarListaNoOrdenada("Producto eliminado: "+ QString::fromStdString(codigo), "green");
}


/*Metodo que permite listar los productos*/
void Controlador::listarProductos(){

    /*Pendiente*/
    emit logListarArbolAvl("Producto listado: ", "green");
    emit logListarListaOrdenada("Producto listado: ", "green");
    emit logListarListaNoOrdenada("Producto listado: ", "green");

}

/*Metodo que permite obtener los datos para poder descargar el Log de errores*/
void Controlador::prepararLogParaDescarga(){
    QString contenido = this->gestorBackend->generarContenidoLog();

    emit logDescargar(contenido);
}


/*Metodo que permite evaluar el estado del boton para poder cerrar el archivo csv cargado*/
void Controlador::evaluarEstadoCerrarCsv(){
    emit evaluarCerrarCsv(this->gestorBackend->getCargoArchivo());
}


/*Metodo que verifica si hay errores*/
void Controlador::verificarErrores(){
    emit evaluarErroresLog(this->gestorBackend->tieneErrores());
}

/*Metodo que verifica  si se cargo ya un csv*/
void Controlador::verificarRefrescado(){
    emit refrescarDatos(this->gestorBackend->getCargoArchivo());
}


/*-----*---Apartado de metodos para poder generar las vistas de los arboles---*-----*/

void Controlador::obtenerArbolAvl(){
    emit enviarArbolAvl(this->gestorBackend->getRaizArbolAvl());
}

void Controlador::obtenerArbolB(){
    emit enviarArbolB(this->gestorBackend->getRaizArbolB());
}

void Controlador::obtenerArbolBMas(){
    emit enviarArbolBMas(new int(1));
}

/*-----*---Fin del Apartado de metodos para poder generar las vistas de los arboles---*-----*/


/*-----*---Apartado de metodos para poder generar los graphviz de los arboles---*-----*/
void Controlador::generarGraphArbolAvl(){
    emit enviarGraphvizArbolAvl(this->gestorBackend->obtenerGraphvizAvl());
}

void Controlador::generarGraphArbolB(){
    emit enviarGraphvizArbolB(this->gestorBackend->obtenerGraphvizB());
}

/*-----*---Fin del apartado de metodos para poder generar los graphviz de los arboles---*-----*/
