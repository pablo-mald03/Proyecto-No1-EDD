#include "controlador.h"
#include "deleteexception.h"
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

            datosValidados.push_back(linea.trimmed());

            emit logCargaCsv(linea.trimmed(), "white");

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

        std::string nombre = columnas[0].trimmed().toStdString();
        std::string key = columnas[1].trimmed().toStdString();
        std::string categoria = columnas[2].trimmed().toStdString();
        std::string fechaExp = columnas[3].trimmed().toStdString();
        std::string marca = columnas[4].trimmed().toStdString();

        bool okPrecio;
        bool okStock;

        double precio = columnas[5].trimmed().toDouble(&okPrecio);
        int stock = columnas[6].trimmed().toInt(&okStock);

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

    double tiempo = timer.nsecsElapsed() / 1000000.0;
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

        std::string nombre = columnas[0].trimmed().toStdString();
        std::string key = columnas[1].trimmed().toStdString();
        std::string categoria = columnas[2].trimmed().toStdString();
        std::string fechaExp = columnas[3].trimmed().toStdString();
        std::string marca = columnas[4].trimmed().toStdString();

        bool okPrecio;
        bool okStock;

        double precio = columnas[5].trimmed().toDouble(&okPrecio);
        int stock = columnas[6].trimmed().toInt(&okStock);

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

    double tiempo = timer.nsecsElapsed() / 1000000.0;
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

        std::string nombre = columnas[0].trimmed().toStdString();
        std::string key = columnas[1].trimmed().toStdString();
        std::string categoria = columnas[2].trimmed().toStdString();
        std::string fechaExp = columnas[3].trimmed().toStdString();
        std::string marca = columnas[4].trimmed().toStdString();

        bool okPrecio;
        bool okStock;

        double precio = columnas[5].trimmed().toDouble(&okPrecio);
        int stock = columnas[6].trimmed().toInt(&okStock);

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

    double tiempo = timer.nsecsElapsed() / 1000000.0;
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

        std::string nombre = columnas[0].trimmed().toStdString();
        std::string key = columnas[1].trimmed().toStdString();
        std::string categoria = columnas[2].trimmed().toStdString();
        std::string fechaExp = columnas[3].trimmed().toStdString();
        std::string marca = columnas[4].trimmed().toStdString();

        bool okPrecio;
        bool okStock;

        double precio = columnas[5].trimmed().toDouble(&okPrecio);
        int stock = columnas[6].trimmed().toInt(&okStock);

        if (!okPrecio || !okStock) {
            emit logLista("Error en conversion numerica: " + linea, "red");
            continue;
        }
        try{

            this->gestorBackend->insertarListasCsv(nombre,key,categoria,fechaExp,marca,precio,stock);

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

    double tiempo = timer.nsecsElapsed() / 1000000.0;
    emit tiempoProceso(4, tiempo);
}

/*Metodo que permite insertar los productos
*
* 1 -> AVL
* 2 -> B
* 3 -> B+
* 4 -> LISTA ORDENADA
* 5 -> LISTA NO ORDENADA
*
*/
void Controlador::insercionProducto(const std::string &_nombre,const std::string &_codigoBarra, const std::string &_categoria, const std::string &_fechaExpiracion, const std::string &_marca, const std::string &_precio, const std::string &_stock){

    try{
        this->gestorBackend->validarInsercion(_nombre,_codigoBarra,_categoria,_fechaExpiracion,_marca,_precio,_stock);

        bool okPrecio;
        bool okStock;

        double precio = QString::fromStdString(_precio).toDouble(&okPrecio);
        int stock = QString::fromStdString(_stock).toInt(&okStock);

        if (!okPrecio || !okStock) {
            throw InsertException("Error de formato: Precio o Stock no son numericos.");
        }

        this->insertarEnListas(_nombre,_codigoBarra,_categoria,_fechaExpiracion,_marca,precio,stock);
        this->insertarEnArbolAvl(_nombre,_codigoBarra,_categoria,_fechaExpiracion,_marca,precio,stock);
        this->insertarEnArbolB(_nombre,_codigoBarra,_categoria,_fechaExpiracion,_marca,precio,stock);
        this->insertarEnArbolBMas(_nombre,_codigoBarra,_categoria,_fechaExpiracion,_marca,precio,stock);

        /*Ordenar lista despues de medir tiempos*/
        this->gestorBackend->ordenarLista(1);

    }catch (const InsertException& e) {
        emit logInsertListaOrdenada( QString::fromStdString(e.what()), "red");
        emit logInsertListaNoOrdenada( QString::fromStdString(e.what()), "red");
        emit logInsertArbolAvl( QString::fromStdString(e.what()), "red");
        emit logInsertArbolB( QString::fromStdString(e.what()), "red");
        emit logInsertArbolBMas( QString::fromStdString(e.what()), "red");
    }
    catch (const std::exception& ex) {
        emit logInsertListaOrdenada("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logInsertListaNoOrdenada("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logInsertArbolAvl("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logInsertArbolB("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logInsertArbolBMas("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
    }

}

/*-------Apartado de metodos de insercion de datos (Rollback)-------*/
/*Metodo que permite insertar en las listas*/
void Controlador::insertarEnListas(const std::string &_nombre,const std::string &_codigoBarra, const std::string &_categoria, const std::string &_fechaExpiracion, const std::string &_marca, double _precio, int _stock){

    try{

        QElapsedTimer timer1;
        timer1.start();

        this->gestorBackend->insertarListaNoOrdenada(_nombre,_codigoBarra,_categoria,_fechaExpiracion,_marca,_precio,_stock);
        emit logInsertListaNoOrdenada("Producto insertaro correctamente: {" + QString::fromStdString(_codigoBarra)+ "}", "green");
        double tiempo1 = timer1.nsecsElapsed() / 1000000.0;
        emit tiempoProcesoInsert(5, tiempo1);


        QElapsedTimer timer2;
        timer2.start();

        this->gestorBackend->insertarListaOrdenada(_nombre,_codigoBarra,_categoria,_fechaExpiracion,_marca,_precio,_stock);
        emit logInsertListaOrdenada("Producto insertaro correctamente: {" + QString::fromStdString(_codigoBarra)+ "}", "green");

        double tiempo2 = timer2.nsecsElapsed() / 1000000.0;

        emit tiempoProcesoInsert(4, tiempo2);

    }catch (const InsertException& e) {
        throw;
    }
    catch (const std::exception& ex) {
        throw InsertException("Error inesperado en listas: " + std::string(ex.what()));
    }

}

/*Metodo que permite insertar en el arbol AVL*/
void Controlador::insertarEnArbolAvl(const std::string &_nombre,const std::string &_codigoBarra, const std::string &_categoria, const std::string &_fechaExpiracion, const std::string &_marca, double _precio, int _stock){

    try{
        QElapsedTimer timer;
        timer.start();

        this->gestorBackend->insertarArbolAvl(_nombre,_codigoBarra,_categoria,_fechaExpiracion,_marca,_precio,_stock);
        emit logInsertArbolAvl("Insertado: {" + QString::fromStdString(_codigoBarra)+ "}" , "green");

        double tiempo = timer.nsecsElapsed() / 1000000.0;

        emit tiempoProcesoInsert(1, tiempo);

    }catch (const InsertException& e) {
        throw;
    }
    catch (const std::exception& ex) {
        throw InsertException("Error inesperado en el arbol AVL: " + std::string(ex.what()));
    }
}

/*Metodo que permite insertar en el arbol B*/
void Controlador::insertarEnArbolB(const std::string &_nombre,const std::string &_codigoBarra, const std::string &_categoria, const std::string &_fechaExpiracion, const std::string &_marca, double _precio, int _stock){

    try{
        QElapsedTimer timer;
        timer.start();

        this->gestorBackend->insertarArbolB(_nombre,_codigoBarra,_categoria,_fechaExpiracion,_marca,_precio,_stock);

        emit logInsertArbolB("Insertado: {" + QString::fromStdString(_codigoBarra)+ "}" , "green");

        double tiempo = timer.nsecsElapsed() / 1000000.0;

        emit tiempoProcesoInsert(2, tiempo);

    }catch (const InsertException& e) {
        throw;
    }
    catch (const std::exception& ex) {
        throw InsertException("Error inesperado en el arbol B: " + std::string(ex.what()));
    }
}

/*Metodo que permite insertar en el arbol B+*/
void Controlador::insertarEnArbolBMas(const std::string &_nombre,const std::string &_codigoBarra, const std::string &_categoria, const std::string &_fechaExpiracion, const std::string &_marca, double _precio, int _stock){

    try{

        QElapsedTimer timer;
        timer.start();

        this->gestorBackend->insertarArbolBMas(_nombre,_codigoBarra,_categoria,_fechaExpiracion,_marca,_precio,_stock);

        emit logInsertArbolBMas("Insertado: {" + QString::fromStdString(_codigoBarra)+ "}" , "green");

        double tiempo = timer.nsecsElapsed() / 1000000.0;

        emit tiempoProcesoInsert(3, tiempo);
    }catch (const InsertException& e) {
        throw;
    }
    catch (const std::exception& ex) {
        throw InsertException("Error inesperado en el arbol B+: " + std::string(ex.what()));
    }
}

/*-------Apartado de metodos de insercion de datos (Rollback)-------*/


/*-------Apartado de metodos que permiten buscar los productos por nombre-------*/
void Controlador::buscarPorNombre(const std::string &nombre){

    try{

        this->buscarAvlNombre(nombre);
        this->buscarListasNombre(nombre);

    }catch (const std::exception& ex) {
        emit logBusquedaNombreArbolAvl("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logBusquedaNombreListaOrdenada("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logBusquedaNombreListaNoOrdenada("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
    }

}

/*Metodos auxiliares que permiten generar las busquedas en la lista ordenada, la lista no ordenada y el arbol AVL*/
void Controlador::buscarAvlNombre(const std::string &nombre){

    QElapsedTimer timer;
    timer.start();
    ListaEnlazada<Producto> lista = this->gestorBackend->buscarProductoAvl(nombre);

    double tiempo = timer.nsecsElapsed() / 1000000.0;

    if(lista.esVacia()){
        emit logBusquedaNombreArbolAvl("No se han encontrado productos con el nombre: "+ QString::fromStdString(nombre), "yellow");
        emit tiempoProcesoBusquedaNombre(1,tiempo);
        return;
    }

    QString cantidad = "==========================================\n"
                       "Productos encontrados:   " + QString::number(lista.getLongitud()) + "\n" +
                       "==========================================\n\n";

    emit logBusquedaNombreArbolAvl(cantidad.replace("\n", "<br>"), "cyan");

    for(int i = 0; i < lista.getLongitud(); i++) {
        Producto product = lista.getValor(i);

        QString logMensaje = "------------------------------------------\n"
                             "Producto:    " + QString::fromStdString(product.getNombre()) + "\n" +
                             "Codigo Barra:      " + QString::fromStdString(product.getCodigoBarra()) + "\n" +
                             "Categoria:   " + QString::fromStdString(product.getCategoria()) + "\n" +
                             "Expiracion:  " + QString::fromStdString(product.getFechaExpiracion()) + "\n" +
                             "Marca:       " + QString::fromStdString(product.getMarca()) + "\n" +
                             "Precio:      $" + QString::number(product.getPrecio(), 'f', 2) + "\n" +
                             "Stock:       " + QString::number(product.getStock()) + " unidades\n" +
                             "------------------------------------------\n";

        emit logBusquedaNombreArbolAvl(logMensaje.replace("\n", "<br>"), "cyan");
    }

    emit tiempoProcesoBusquedaNombre(1, tiempo);
}

/*Permite listar por nombre*/
void Controlador::buscarListasNombre(const std::string &nombre){

    QElapsedTimer timerOrdenada;
    timerOrdenada.start();
    ListaEnlazada<Producto> listaOrdenada = this->gestorBackend->buscarProductoListaOrdenada(nombre);

    double tiempoOrdenado = timerOrdenada.nsecsElapsed() / 1000000.0;

    if(listaOrdenada.esVacia()){
        emit logBusquedaNombreListaOrdenada("No se han encontrado productos con el nombre: "+ QString::fromStdString(nombre), "yellow");
        emit tiempoProcesoBusquedaNombre(2,tiempoOrdenado);
    }else{

        QString cantidadLista = "==========================================\n"
                           "Productos encontrados:   " + QString::number(listaOrdenada.getLongitud()) + "\n" +
                           "==========================================\n\n";

        emit logBusquedaNombreListaOrdenada(cantidadLista.replace("\n", "<br>"), "cyan");

        for(int i = 0; i < listaOrdenada.getLongitud(); i++) {
            Producto product = listaOrdenada.getValor(i);

            QString logMensaje = "------------------------------------------\n"
                                 "Producto:    " + QString::fromStdString(product.getNombre()) + "\n" +
                                 "Codigo Barra:      " + QString::fromStdString(product.getCodigoBarra()) + "\n" +
                                 "Categoria:   " + QString::fromStdString(product.getCategoria()) + "\n" +
                                 "Expiracion:  " + QString::fromStdString(product.getFechaExpiracion()) + "\n" +
                                 "Marca:       " + QString::fromStdString(product.getMarca()) + "\n" +
                                 "Precio:      $" + QString::number(product.getPrecio(), 'f', 2) + "\n" +
                                 "Stock:       " + QString::number(product.getStock()) + " unidades\n" +
                                 "------------------------------------------\n";

            emit logBusquedaNombreListaOrdenada(logMensaje.replace("\n", "<br>"), "cyan");
        }

        emit tiempoProcesoBusquedaNombre(2, tiempoOrdenado);
    }

    QElapsedTimer timer;
    timer.start();
    ListaEnlazada<Producto> listaNoOrdenada = this->gestorBackend->buscarProductoListaNoOrdenada(nombre);

    double tiempo = timer.nsecsElapsed() / 1000000.0;

    if(listaNoOrdenada.esVacia()){
        emit logBusquedaNombreListaNoOrdenada("No se han encontrado productos con el nombre: "+ QString::fromStdString(nombre), "yellow");
        emit tiempoProcesoBusquedaNombre(3,tiempo);

    }else{

        QString cantidad = "==========================================\n"
                           "Productos encontrados:   " + QString::number(listaNoOrdenada.getLongitud()) + "\n" +
                           "==========================================\n\n";

        emit logBusquedaNombreListaNoOrdenada(cantidad.replace("\n", "<br>"), "cyan");

        for(int i = 0; i < listaNoOrdenada.getLongitud(); i++) {
            Producto product = listaNoOrdenada.getValor(i);

            QString logMensaje = "------------------------------------------\n"
                                 "Producto:    " + QString::fromStdString(product.getNombre()) + "\n" +
                                 "Codigo Barra:      " + QString::fromStdString(product.getCodigoBarra()) + "\n" +
                                 "Categoria:   " + QString::fromStdString(product.getCategoria()) + "\n" +
                                 "Expiracion:  " + QString::fromStdString(product.getFechaExpiracion()) + "\n" +
                                 "Marca:       " + QString::fromStdString(product.getMarca()) + "\n" +
                                 "Precio:      $" + QString::number(product.getPrecio(), 'f', 2) + "\n" +
                                 "Stock:       " + QString::number(product.getStock()) + " unidades\n" +
                                 "------------------------------------------\n";

            emit logBusquedaNombreListaNoOrdenada(logMensaje.replace("\n", "<br>"), "cyan");
        }

        emit tiempoProcesoBusquedaNombre(3, tiempo);
    }
}


/*-------Fin del apartado de Metodos que permiten buscar los productos por nombre-------*/


/*-------Apartado de Metodos que permiten buscar los productos por categoria-------*/

/*Metodo que permite buscar los productos por categoria*/
void Controlador::buscarPorCategoria(std::string categoria){

    try{

        this->buscarBMasCategoria(categoria);
        this->buscarListasCategoria(categoria);

    }catch (const std::exception& ex) {
        emit logBusquedaCategoriaArbolBMas("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logBusquedaCategoriaListaOrdenada("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logBusquedaCategoriaListaNoOrdenada("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
    }
}

/*Metodos auxiliares que permiten generar las busquedas en la lista ordenada, la lista no ordenada y el arbol AVL*/
void Controlador::buscarBMasCategoria(const std::string &categoria){

    QElapsedTimer timer;
    timer.start();
    ListaEnlazada<Producto> lista = this->gestorBackend->buscarProductoBMas(categoria);

    double tiempo = timer.nsecsElapsed() / 1000000.0;

    if(lista.esVacia()){
        emit logBusquedaCategoriaArbolBMas("No se han encontrado productos con la categoria: "+ QString::fromStdString(categoria), "yellow");
        emit tiempoProcesoBusquedaCategoria(1,tiempo);
        return;
    }

    QString cantidad = "==========================================\n"
                       "Productos encontrados:   " + QString::number(lista.getLongitud()) + "\n" +
                       "==========================================\n\n";

    emit logBusquedaCategoriaArbolBMas(cantidad.replace("\n", "<br>"), "cyan");

    for(int i = 0; i < lista.getLongitud(); i++) {
        Producto product = lista.getValor(i);

        QString logMensaje = "------------------------------------------\n"
                             "Categoria:   " + QString::fromStdString(product.getCategoria()) + "\n" +
                             "Producto:    " + QString::fromStdString(product.getNombre()) + "\n" +
                             "Codigo Barra:      " + QString::fromStdString(product.getCodigoBarra()) + "\n" +
                             "Expiracion:  " + QString::fromStdString(product.getFechaExpiracion()) + "\n" +
                             "Marca:       " + QString::fromStdString(product.getMarca()) + "\n" +
                             "Precio:      $" + QString::number(product.getPrecio(), 'f', 2) + "\n" +
                             "Stock:       " + QString::number(product.getStock()) + " unidades\n" +
                             "------------------------------------------\n";

        emit logBusquedaCategoriaArbolBMas(logMensaje.replace("\n", "<br>"), "cyan");
    }

    emit tiempoProcesoBusquedaCategoria(1, tiempo);
}

/*Permite listar por categoria*/
void Controlador::buscarListasCategoria(const std::string &categoria){

    QElapsedTimer timerOrdenada;
    timerOrdenada.start();
    ListaEnlazada<Producto> listaOrdenada = this->gestorBackend->buscarProductoCategoriaListaOrdenada(categoria);

    double tiempoOrdenado = timerOrdenada.nsecsElapsed() / 1000000.0;

    if(listaOrdenada.esVacia()){
        emit logBusquedaCategoriaListaOrdenada("No se han encontrado productos con la categoria: "+ QString::fromStdString(categoria), "yellow");
        emit tiempoProcesoBusquedaCategoria(2,tiempoOrdenado);
    }else{

        QString cantidad = "==========================================\n"
                           "Productos encontrados:   " + QString::number(listaOrdenada.getLongitud()) + "\n" +
                           "==========================================\n\n";

        emit logBusquedaCategoriaListaOrdenada(cantidad.replace("\n", "<br>"), "cyan");

        for(int i = 0; i < listaOrdenada.getLongitud(); i++) {
            Producto product = listaOrdenada.getValor(i);

            QString logMensaje = "------------------------------------------\n"
                                 "Categoria:   " + QString::fromStdString(product.getCategoria()) + "\n" +
                                 "Producto:    " + QString::fromStdString(product.getNombre()) + "\n" +
                                 "Codigo Barra:      " + QString::fromStdString(product.getCodigoBarra()) + "\n" +
                                 "Expiracion:  " + QString::fromStdString(product.getFechaExpiracion()) + "\n" +
                                 "Marca:       " + QString::fromStdString(product.getMarca()) + "\n" +
                                 "Precio:      $" + QString::number(product.getPrecio(), 'f', 2) + "\n" +
                                 "Stock:       " + QString::number(product.getStock()) + " unidades\n" +
                                 "------------------------------------------\n";

            emit logBusquedaCategoriaListaOrdenada(logMensaje.replace("\n", "<br>"), "cyan");
        }

        emit tiempoProcesoBusquedaCategoria(2, tiempoOrdenado);
    }

    QElapsedTimer timer;
    timer.start();
    ListaEnlazada<Producto> listaNoOrdenada = this->gestorBackend->buscarProductoCategoriaListaNoOrdenada(categoria);

    double tiempo = timer.nsecsElapsed() / 1000000.0;

    if(listaNoOrdenada.esVacia()){
        emit logBusquedaCategoriaListaNoOrdenada("No se han encontrado productos con la categoria: "+ QString::fromStdString(categoria), "yellow");
        emit tiempoProcesoBusquedaCategoria(3,tiempo);

    }else{

        QString cantidadLista = "==========================================\n"
                           "Productos encontrados:   " + QString::number(listaNoOrdenada.getLongitud()) + "\n" +
                           "==========================================\n\n";

        emit logBusquedaCategoriaListaNoOrdenada(cantidadLista.replace("\n", "<br>"), "cyan");

        for(int i = 0; i < listaNoOrdenada.getLongitud(); i++) {
            Producto product = listaNoOrdenada.getValor(i);

            QString logMensaje = "------------------------------------------\n"
                                 "Categoria:   " + QString::fromStdString(product.getCategoria()) + "\n" +
                                 "Producto:    " + QString::fromStdString(product.getNombre()) + "\n" +
                                 "Codigo Barra:      " + QString::fromStdString(product.getCodigoBarra()) + "\n" +
                                 "Expiracion:  " + QString::fromStdString(product.getFechaExpiracion()) + "\n" +
                                 "Marca:       " + QString::fromStdString(product.getMarca()) + "\n" +
                                 "Precio:      $" + QString::number(product.getPrecio(), 'f', 2) + "\n" +
                                 "Stock:       " + QString::number(product.getStock()) + " unidades\n" +
                                 "------------------------------------------\n";

            emit logBusquedaCategoriaListaNoOrdenada(logMensaje.replace("\n", "<br>"), "cyan");
        }

        emit tiempoProcesoBusquedaCategoria(3, tiempo);
    }
}

/*-------Fin del apartado de Metodos que permiten buscar los productos por categoria-------*/


/*-------Apartado de Metodos que permiten buscar los productos por fechas-------*/

/*Metodo que permite buscar los productos por fechas*/
void Controlador::buscarPorFecha(const std::string &limiteInferior, const std::string &limiteSuperior){

    try{

        this->buscarBFecha(limiteInferior, limiteSuperior);
        this->buscarListasFecha(limiteInferior, limiteSuperior);

    }catch (const std::exception& ex) {
        emit logBusquedaFechasArbolB("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logBusquedaFechasListaOrdenada("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logBusquedaFechasListaNoOrdenada("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
    }

}


/*Metodos auxiliares que permiten generar las busquedas en la lista ordenada, la lista no ordenada y el arbol AVL*/
void Controlador::buscarBFecha(const std::string &limiteInferior, const std::string &limiteSuperior){

    QElapsedTimer timer;
    timer.start();
    ListaEnlazada<Producto> lista = this->gestorBackend->buscarProductoB(limiteInferior, limiteSuperior);

    double tiempo = timer.nsecsElapsed() / 1000000.0;

    if(lista.esVacia()){
        emit logBusquedaFechasArbolB("No se han encontrado productos en el intervalo: "+ QString::fromStdString(limiteInferior)+" - " + QString::fromStdString(limiteSuperior), "yellow");
        emit tiempoProcesoBusquedaFechas(1,tiempo);
        return;
    }

    QString cantidad = "==========================================\n"
                       "Productos encontrados:   " + QString::number(lista.getLongitud()) + "\n" +
                       "==========================================\n\n";

    emit logBusquedaFechasArbolB(cantidad.replace("\n", "<br>"), "cyan");

    for(int i = 0; i < lista.getLongitud(); i++) {
        Producto product = lista.getValor(i);

        QString logMensaje = "------------------------------------------\n"
                             "Fecha Expiracion:  " + QString::fromStdString(product.getFechaExpiracion()) + "\n" +
                             "Producto:    " + QString::fromStdString(product.getNombre()) + "\n" +
                             "Codigo Barra:      " + QString::fromStdString(product.getCodigoBarra()) + "\n" +
                             "Categoria:   " + QString::fromStdString(product.getCategoria()) + "\n" +
                             "Marca:       " + QString::fromStdString(product.getMarca()) + "\n" +
                             "Precio:      $" + QString::number(product.getPrecio(), 'f', 2) + "\n" +
                             "Stock:       " + QString::number(product.getStock()) + " unidades\n" +
                             "------------------------------------------\n";

        emit logBusquedaFechasArbolB(logMensaje.replace("\n", "<br>"), "cyan");
    }

    emit tiempoProcesoBusquedaFechas(1, tiempo);
}

/*Permite listar por rango de fechas*/
void Controlador::buscarListasFecha(const std::string &limiteInferior, const std::string &limiteSuperior){

    QElapsedTimer timerOrdenada;
    timerOrdenada.start();
    ListaEnlazada<Producto> listaOrdenada = this->gestorBackend->buscarProductoFechasListaOrdenada(limiteInferior, limiteSuperior);

    double tiempoOrdenado = timerOrdenada.nsecsElapsed() / 1000000.0;

    if(listaOrdenada.esVacia()){
        emit logBusquedaFechasListaOrdenada("No se han encontrado productos en el intervalo: "+ QString::fromStdString(limiteInferior)+" - " + QString::fromStdString(limiteSuperior), "yellow");
        emit tiempoProcesoBusquedaFechas(2,tiempoOrdenado);
    }else{

        QString cantidad = "==========================================\n"
                           "Productos encontrados:   " + QString::number(listaOrdenada.getLongitud()) + "\n" +
                           "==========================================\n\n";

        emit logBusquedaFechasListaOrdenada(cantidad.replace("\n", "<br>"), "cyan");

        for(int i = 0; i < listaOrdenada.getLongitud(); i++) {
            Producto product = listaOrdenada.getValor(i);

            QString logMensaje = "------------------------------------------\n"
                                 "Fecha Expiracion:  " + QString::fromStdString(product.getFechaExpiracion()) + "\n" +
                                 "Producto:    " + QString::fromStdString(product.getNombre()) + "\n" +
                                 "Codigo Barra:      " + QString::fromStdString(product.getCodigoBarra()) + "\n" +
                                 "Categoria:   " + QString::fromStdString(product.getCategoria()) + "\n" +
                                 "Marca:       " + QString::fromStdString(product.getMarca()) + "\n" +
                                 "Precio:      $" + QString::number(product.getPrecio(), 'f', 2) + "\n" +
                                 "Stock:       " + QString::number(product.getStock()) + " unidades\n" +
                                 "------------------------------------------\n";

            emit logBusquedaFechasListaOrdenada(logMensaje.replace("\n", "<br>"), "cyan");
        }

        emit tiempoProcesoBusquedaFechas(2, tiempoOrdenado);
    }

    QElapsedTimer timer;
    timer.start();
    ListaEnlazada<Producto> listaNoOrdenada = this->gestorBackend->buscarProductoFechasListaNoOrdenada(limiteInferior, limiteSuperior);

    double tiempo = timer.nsecsElapsed() / 1000000.0;

    if(listaNoOrdenada.esVacia()){
        emit logBusquedaFechasListaNoOrdenada("No se han encontrado productos en el intervalo: "+ QString::fromStdString(limiteInferior)+" - " + QString::fromStdString(limiteSuperior), "yellow");
        emit tiempoProcesoBusquedaFechas(3,tiempo);

    }else{

        QString cantidadLista = "==========================================\n"
                                "Productos encontrados:   " + QString::number(listaNoOrdenada.getLongitud()) + "\n" +
                                "==========================================\n\n";

        emit logBusquedaFechasListaNoOrdenada(cantidadLista.replace("\n", "<br>"), "cyan");

        for(int i = 0; i < listaNoOrdenada.getLongitud(); i++) {
            Producto product = listaNoOrdenada.getValor(i);

            QString logMensaje = "------------------------------------------\n"
                                 "Fecha Expiracion:  " + QString::fromStdString(product.getFechaExpiracion()) + "\n" +
                                 "Producto:    " + QString::fromStdString(product.getNombre()) + "\n" +
                                 "Codigo Barra:      " + QString::fromStdString(product.getCodigoBarra()) + "\n" +
                                 "Categoria:   " + QString::fromStdString(product.getCategoria()) + "\n" +
                                 "Marca:       " + QString::fromStdString(product.getMarca()) + "\n" +
                                 "Precio:      $" + QString::number(product.getPrecio(), 'f', 2) + "\n" +
                                 "Stock:       " + QString::number(product.getStock()) + " unidades\n" +
                                 "------------------------------------------\n";

            emit logBusquedaFechasListaNoOrdenada(logMensaje.replace("\n", "<br>"), "cyan");
        }

        emit tiempoProcesoBusquedaFechas(3, tiempo);
    }
}



/*Metodo que permite eliminar un producto por codigo*/
void Controlador::eliminarProducto(const std::string &codigo){

    try{

        this->eliminarEnListas(codigo);
        qDebug()<<"Eliminaron las listas";
        this->eliminarEnArbolAvl(codigo);
        qDebug()<<"Elimino el AVL";
        this->eliminarEnArbolB(codigo);
        qDebug()<<"Elimino el B";
        this->eliminarEnArbolBMas(codigo);
        qDebug()<<"Elimino el BMas";

        /*Ordenar lista despues de medir tiempos*/
        this->gestorBackend->ordenarLista(1);

    }catch (const InsertException& e) {
        emit logEliminarListaOrdenada( QString::fromStdString(e.what()), "red");
        emit logEliminarListaNoOrdenada( QString::fromStdString(e.what()), "red");
        emit logEliminarArbolAvl( QString::fromStdString(e.what()), "red");
        emit logEliminarArbolB( QString::fromStdString(e.what()), "red");
        emit logEliminarArbolBMas( QString::fromStdString(e.what()), "red");
    }
    catch (const DeleteException& ex) {
        emit logEliminarListaOrdenada( QString::fromStdString(ex.what()), "red");
        emit logEliminarListaNoOrdenada( QString::fromStdString(ex.what()), "red");
        emit logEliminarArbolAvl( QString::fromStdString(ex.what()), "red");
        emit logEliminarArbolB( QString::fromStdString(ex.what()), "red");
        emit logEliminarArbolBMas( QString::fromStdString(ex.what()), "red");
    }
    catch (const std::exception& ex) {
        emit logEliminarListaNoOrdenada("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logEliminarListaNoOrdenada("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logEliminarArbolAvl("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logEliminarArbolB("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logEliminarArbolBMas("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
    }
}


/*------APARTADO DE AUXILIARES QUE PERMITEN ELIMINAR LOS PRODUCTOS------*/
/*Metodo que permite insertar en las listas*/
void Controlador::eliminarEnListas(const std::string &_codigoBarra){

    try{

        QElapsedTimer timer1;
        timer1.start();

        this->gestorBackend->eliminarListaNoOrdenada(_codigoBarra);
        emit logEliminarListaNoOrdenada("Producto eliminado correctamente con el codigo: {" + QString::fromStdString(_codigoBarra)+ "}", "green");
        double tiempo1 = timer1.nsecsElapsed() / 1000000.0;
        emit tiempoEliminarProceso(5, tiempo1);


        QElapsedTimer timer2;
        timer2.start();

        this->gestorBackend->eliminarListaOrdenada(_codigoBarra);
        emit logEliminarListaOrdenada("Producto eliminado correctamente con el codigo: {" + QString::fromStdString(_codigoBarra)+ "}", "green");

        double tiempo2 = timer2.nsecsElapsed() / 1000000.0;

        emit tiempoEliminarProceso(4, tiempo2);

    }catch (const DeleteException& e) {
        throw;
    }
    catch (const std::exception& ex) {
        throw DeleteException("Error inesperado en listas: " + std::string(ex.what()));
    }

}

/*Metodo que permite eliminar en el arbol AVL*/
void Controlador::eliminarEnArbolAvl(const std::string &_codigoBarra){

    try{
        QElapsedTimer timer;
        timer.start();

        this->gestorBackend->eliminarArbolAvl(_codigoBarra);

        emit logEliminarArbolAvl("Producto eliminado correctamente con el codigo: {" + QString::fromStdString(_codigoBarra)+ "}" , "green");

        double tiempo = timer.nsecsElapsed() / 1000000.0;

        emit tiempoEliminarProceso(1, tiempo);

    }catch (const DeleteException& e) {
        throw;
    }
    catch (const std::exception& ex) {
        throw DeleteException("Error inesperado en el arbol AVL: " + std::string(ex.what()));
    }
}

/*Metodo que permite eliminar en el arbol B*/
void Controlador::eliminarEnArbolB(const std::string &_codigoBarra){

    try{
        QElapsedTimer timer;
        timer.start();

        this->gestorBackend->eliminarArbolB(_codigoBarra);

        emit logEliminarArbolB("Producto eliminado correctamente con el codigo: {" + QString::fromStdString(_codigoBarra)+ "}" , "green");

        double tiempo = timer.nsecsElapsed() / 1000000.0;

        emit tiempoEliminarProceso(2, tiempo);

    }catch (const DeleteException& e) {
        throw;
    }
    catch (const std::exception& ex) {
        throw DeleteException("Error inesperado en el arbol B: " + std::string(ex.what()));
    }
}

/*Metodo que permite eliminar en el arbol B+*/
void Controlador::eliminarEnArbolBMas(const std::string &_codigoBarra){

    try{

        QElapsedTimer timer;
        timer.start();

        this->gestorBackend->eliminarArbolBMas(_codigoBarra);

        emit logEliminarArbolBMas("Producto eliminado correctamente con el codigo: {" + QString::fromStdString(_codigoBarra)+ "}" , "green");

        double tiempo = timer.nsecsElapsed() / 1000000.0;

        emit tiempoEliminarProceso(3, tiempo);

    }catch (const DeleteException& e) {
        throw;
    }
    catch (const std::exception& ex) {
        throw DeleteException("Error inesperado en el arbol B+: " + std::string(ex.what()));
    }
}

/*------FIN DEL APARTADO DE AUXILIARES QUE PERMITEN ELIMINAR LOS PRODUCTOS------*/


/*------APARTADO DE METODOS QUE PERMITEN LISTAR EN ORDEN ALFABETICO LOS PRODUCTOS------*/

/*Metodo que permite listar los productos*/
void Controlador::listarProductos(){
    try{
        this->listarAlfabeticoAvl();
        this->listarAlfabeticoListas();
    }catch (const std::exception& ex) {
        emit logListarArbolAvl("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logListarListaOrdenada("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
        emit logListarListaNoOrdenada("Error inesperado: " + QString::fromStdString(ex.what()) , "red");
    }
}

/*Metodo que permite listar el producto nombre en orden alfabetico en base a los resultados del arbol AVL*/
void Controlador::listarAlfabeticoAvl(){

    QElapsedTimer timer;
    timer.start();
    ListaEnlazada<Producto> lista = this->gestorBackend->listarProductosAvl();

    double tiempo = timer.nsecsElapsed() / 1000000.0;

    if(lista.esVacia()){
        emit logListarArbolAvl("El arbol AVL esta vacio.", "yellow");
        emit tiempoProcesoListarNombre(1,tiempo);
        return;
    }

    QString cantidad = "==========================================\n"
                       "Productos listados:   " + QString::number(lista.getLongitud()) + "\n" +
                       "==========================================\n\n";

    emit logListarArbolAvl(cantidad.replace("\n", "<br>"), "yellow");

    for(int i = 0; i < lista.getLongitud(); i++) {
        Producto product = lista.getValor(i);

        QString logMensaje = "------------------------------------------\n"
                             "Producto:    " + QString::fromStdString(product.getNombre()) + "\n" +
                             "Codigo Barra:      " + QString::fromStdString(product.getCodigoBarra()) + "\n" +
                             "Categoria:   " + QString::fromStdString(product.getCategoria()) + "\n" +
                             "Marca:       " + QString::fromStdString(product.getMarca()) + "\n" +
                             "Fecha Expiracion:  " + QString::fromStdString(product.getFechaExpiracion()) + "\n" +
                             "Precio:      $" + QString::number(product.getPrecio(), 'f', 2) + "\n" +
                             "Stock:       " + QString::number(product.getStock()) + " unidades\n" +
                             "------------------------------------------\n";

        emit logListarArbolAvl(logMensaje.replace("\n", "<br>"), "cyan");
    }

    emit tiempoProcesoListarNombre(1, tiempo);
}

/*Metodo que permite listar el producto nombre en orden alfabetico en base a los resultados de las listas enlazadas*/
void Controlador::listarAlfabeticoListas(){

    QElapsedTimer timerOrdenada;
    timerOrdenada.start();
    ListaEnlazada<Producto> listaOrdenada = this->gestorBackend->listarProductosOrdenados();

    double tiempoOrdenado = timerOrdenada.nsecsElapsed() / 1000000.0;

    if(listaOrdenada.esVacia()){
        emit logListarListaOrdenada("La lista Ordenada esta vacia", "yellow");
        emit tiempoProcesoListarNombre(2,tiempoOrdenado);
    }else{

        QString cantidad = "==========================================\n"
                           "Productos listados:   " + QString::number(listaOrdenada.getLongitud()) + "\n" +
                           "==========================================\n\n";

        emit logListarListaOrdenada(cantidad.replace("\n", "<br>"), "yellow");

        for(int i = 0; i < listaOrdenada.getLongitud(); i++) {
            Producto product = listaOrdenada.getValor(i);

            QString logMensaje = "------------------------------------------\n"
                                 "Producto:    " + QString::fromStdString(product.getNombre()) + "\n" +
                                 "Codigo Barra:      " + QString::fromStdString(product.getCodigoBarra()) + "\n" +
                                 "Categoria:   " + QString::fromStdString(product.getCategoria()) + "\n" +
                                 "Marca:       " + QString::fromStdString(product.getMarca()) + "\n" +
                                 "Fecha Expiracion:  " + QString::fromStdString(product.getFechaExpiracion()) + "\n" +
                                 "Precio:      $" + QString::number(product.getPrecio(), 'f', 2) + "\n" +
                                 "Stock:       " + QString::number(product.getStock()) + " unidades\n" +
                                 "------------------------------------------\n";

            emit logListarListaOrdenada(logMensaje.replace("\n", "<br>"), "cyan");
        }

        emit tiempoProcesoListarNombre(2, tiempoOrdenado);
    }

    QElapsedTimer timer;
    timer.start();
    ListaEnlazada<Producto> listaNoOrdenada = this->gestorBackend->listarProductosNoOrdenados();

    double tiempo = timer.nsecsElapsed() / 1000000.0;

    if(listaNoOrdenada.esVacia()){
        emit logListarListaNoOrdenada("La lista No Ordenada esta vacia", "yellow");
        emit tiempoProcesoListarNombre(3,tiempo);

    }else{

        QString cantidadLista = "==========================================\n"
                                "Productos listados:   " + QString::number(listaNoOrdenada.getLongitud()) + "\n" +
                                "==========================================\n\n";

        emit logListarListaNoOrdenada(cantidadLista.replace("\n", "<br>"), "yellow");

        for(int i = 0; i < listaNoOrdenada.getLongitud(); i++) {
            Producto product = listaNoOrdenada.getValor(i);

            QString logMensaje = "------------------------------------------\n"
                                 "Producto:    " + QString::fromStdString(product.getNombre()) + "\n" +
                                 "Codigo Barra:      " + QString::fromStdString(product.getCodigoBarra()) + "\n" +
                                 "Categoria:   " + QString::fromStdString(product.getCategoria()) + "\n" +
                                 "Marca:       " + QString::fromStdString(product.getMarca()) + "\n" +
                                 "Fecha Expiracion:  " + QString::fromStdString(product.getFechaExpiracion()) + "\n" +
                                 "Precio:      $" + QString::number(product.getPrecio(), 'f', 2) + "\n" +
                                 "Stock:       " + QString::number(product.getStock()) + " unidades\n" +
                                 "------------------------------------------\n";

            emit logListarListaNoOrdenada(logMensaje.replace("\n", "<br>"), "cyan");
        }

        emit tiempoProcesoListarNombre(3, tiempo);
    }
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
    emit enviarArbolBMas(this->gestorBackend->getRaizArbolBMas());
}

/*-----*---Fin del Apartado de metodos para poder generar las vistas de los arboles---*-----*/


/*-----*---Apartado de metodos para poder generar los graphviz de los arboles---*-----*/
void Controlador::generarGraphArbolAvl(){
    emit enviarGraphvizArbolAvl(this->gestorBackend->obtenerGraphvizAvl());
}

void Controlador::generarGraphArbolB(){
    emit enviarGraphvizArbolB(this->gestorBackend->obtenerGraphvizB());
}

void Controlador::generarGraphArbolBMas(){
    emit enviarGraphvizArbolBMas(this->gestorBackend->obtenerGraphvizBMas());
}

/*-----*---Fin del apartado de metodos para poder generar los graphviz de los arboles---*-----*/


/*Metodo que permite ordenar a la lista*/
void Controlador::ordenarListado(int orden){
    this->gestorBackend->ordenarLista(orden);
}
