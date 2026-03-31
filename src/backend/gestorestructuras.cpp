#include "gestorestructuras.h"
#include "insertexception.h"
#include "readercsvexception.h"
#include <ctime>

/*Librerias solo utilizadas como buffers para poder ordenar*/
#include <vector>
#include <sstream>

GestorEstructuras::GestorEstructuras():
    listaNoOrdenada(new ListaEnlazada<Producto>()),
    listaOrdenada(new ListaEnlazada<Producto>()),
    listaErrores(new ListaEnlazada<ErroresLectura>()),
    arbolAvl(new ArbolAvl()),
    arbolB(new ArbolB(5)),
    arbolBMas(new ArbolBMas(5)),
    cargoArchivo(false)
{
}

/*Destructor*/
GestorEstructuras::~GestorEstructuras(){

    if(this->arbolAvl != nullptr){
        delete this->arbolAvl;
        this->arbolAvl = nullptr;
    }

    if(this->arbolB != nullptr){
        delete this->arbolB;
        this->arbolB = nullptr;
    }


    if(this->arbolBMas != nullptr){
        delete this->arbolBMas;
        this->arbolBMas = nullptr;
    }

    if(this->listaNoOrdenada != nullptr){
        delete this->listaNoOrdenada;
        this->listaNoOrdenada = nullptr;
    }

    if(this->listaOrdenada != nullptr){
        delete this->listaOrdenada;
        this->listaOrdenada = nullptr;
    }

    if(this->listaErrores != nullptr){
        delete this->listaErrores;
        this->listaErrores = nullptr;
    }
}

/*Metodo que permite exportar el csv*/
std::string GestorEstructuras::serializarListaCsv() {
    std::stringstream sStream;

    NodoLista<Producto>* actual = this->listaNoOrdenada->getCabeza();
    while (actual != nullptr) {
        const Producto& p = actual->getDato();

        sStream << "\"" << p.getNombre() << "\","
           << "\"" << p.getCodigoBarra() << "\","
           << "\"" << p.getCategoria() << "\","
           << "\"" << p.getFechaExpiracion() << "\","
           << "\"" << p.getMarca() << "\","
           << p.getPrecio() << ","
           << p.getStock() << "\n";

        actual = actual->getSiguiente();
    }
    return sStream.str();
}


/*----****------Apartado de metodos setter y setters para poder interactuar con las estructuras------****---*/

/*Metodo que permite obtener la raiz del arbol B+*/
NodoBMas * GestorEstructuras::getRaizArbolBMas(){
    return this->arbolBMas->getRaiz();
}

/*Metodo que permite obtener la raiz del arbol B*/
NodoB * GestorEstructuras::getRaizArbolB(){
    return this->arbolB->getRaiz();
}

/*Metodo que permite obtener el .dot del arbol AVL*/
std::string GestorEstructuras::obtenerGraphvizAvl(){
    return this->arbolAvl->generarDot();
}

/*Metodo que permite obtener el .dot del arbol B*/
std::string GestorEstructuras::obtenerGraphvizB(){
    return this->arbolB->generarDot();
}

/*Metodo que permite obtener el .dot del arbol B+*/
std::string GestorEstructuras::obtenerGraphvizBMas(){
    return this->arbolBMas->generarDot();
}

/*Metodo que permite obtener la raiz del arbol avl*/
NodoAvl * GestorEstructuras::getRaizArbolAvl(){
    return this->arbolAvl->getRaiz();
}

/*Getters y setters de la flag*/
void GestorEstructuras::setCargoArchivo(bool carga){
    this->cargoArchivo = carga;
}

bool GestorEstructuras::getCargoArchivo(){
    return this->cargoArchivo;
}

/*Metodo de validacion del csv delegado para validar*/
void GestorEstructuras::agregarErrorLista(const std::string mensaje, int fila){

    this->listaErrores->insertarAtras(ErroresLectura(mensaje, fila));
}

/*Getter de la lista de errores*/
ListaEnlazada<ErroresLectura>* GestorEstructuras::getListaErrores(){

    return this->listaErrores;
}

/*Getter de la lista no ordenada*/
ListaEnlazada<Producto> * GestorEstructuras::getListaNoOrdenada(){
    return this->listaNoOrdenada;
}

/*Metodo para saber si tiene errores la lista*/
bool GestorEstructuras::tieneErrores() const {
    return !this->listaErrores->esVacia();
}

/*----****------Fin del Apartado de metodos setter para poder interactuar con las estrcuturas------****---*/

/*Metodo que permite verificar la integridad de la listaEnlazada*/
bool GestorEstructuras::existeProductoListaNoOrdenada(const std::string &codigo){

    NodoLista<Producto> * actual = this->listaNoOrdenada->getCabeza();

    while (actual != nullptr) {
        if (actual->getDato().getCodigoBarra() == codigo) {
            return true;
        }
        actual = actual->getSiguiente();
    }
    return false;
}


/*Metodo que permite verificar la integridad de la listaEnlazada*/
bool GestorEstructuras::existeProductoListaOrdenada(const std::string &codigo){

    NodoLista<Producto> * actual = this->listaOrdenada->getCabeza();

    while (actual != nullptr) {
        if (actual->getDato().getCodigoBarra() == codigo) {
            return true;
        }
        actual = actual->getSiguiente();
    }
    return false;
}


/*---****-----Apartado de Metodos que permiten utilizar el arbol AVL-------****---*/

/*Metodo que permite insertar datos en el arbol Avl*/
void GestorEstructuras::insertarArbolAvl(std::string nombre, std::string key, std::string categoria, std::string fecha, std::string marca, double precio, int stock){

    try{
        this->arbolAvl->insertar(Producto(nombre,key,categoria,fecha,marca,precio,stock));
    }catch (const InsertException& e) {
        throw InsertException(e.what());
    }
}

/*---****-----Fin del Apartado de Metodos que permiten utilizar el arbol AVL-------****---*/


/*---****-----Apartado de Metodos que permiten utilizar el arbol B-------****---*/

/*Metodo que permite insertar datos en el arbol Avl*/
void GestorEstructuras::insertarArbolB(std::string nombre, std::string key, std::string categoria, std::string fecha, std::string marca, double precio, int stock){

    try{
        this->arbolB->insertar(Producto(nombre,key,categoria,fecha,marca,precio,stock));
    }catch (const InsertException& e) {
        throw InsertException(e.what());
    }
}

/*---****-----Fin del Apartado de Metodos que permiten utilizar el arbol B-------****---*/


/*---****-----Apartado de Metodos que permiten utilizar el arbol B+-------****---*/

/*Metodo que permite insertar datos en el arbol Avl*/
void GestorEstructuras::insertarArbolBMas(std::string nombre, std::string key, std::string categoria, std::string fecha, std::string marca, double precio, int stock){

    try{
        this->arbolBMas->insertar(Producto(nombre,key,categoria,fecha,marca,precio,stock));
    }catch (const InsertException& e) {
        throw InsertException(e.what());
    }
}

/*---****-----Fin del Apartado de Metodos que permiten utilizar el arbol B+-------****---*/



/*Metodo que permite insertar los datos en la lista*/
void GestorEstructuras::insertarListasCsv(const std::string &nombre,const std::string &key,const std::string &categoria, const std::string &fecha, const std::string &marca, double precio, int stock){

    if (this->existeProductoListaNoOrdenada(key)) {

        throw ReaderCsvException("El Codigo de Barra { " + key + " } ya existe para el producto: "+ nombre +" [LISTA ENLAZADA].");
    }

    this->listaNoOrdenada->insertarAtras(Producto(nombre,key,categoria,fecha,marca,precio,stock));
}

/*Metodo que permite insertar datos en la lista no ordenada*/
void GestorEstructuras::insertarListaNoOrdenada(const std::string &nombre,const std::string &key,const std::string &categoria, const std::string &fecha, const std::string &marca, double precio, int stock){

    if (this->existeProductoListaNoOrdenada(key)) {
        throw InsertException("El Codigo de Barra { " + key + " } ya existe para el producto: "+ nombre +" [LISTA NO ORDENADA].");
    }

    this->listaNoOrdenada->insertarAtras(Producto(nombre,key,categoria,fecha,marca,precio,stock));
}

/*Metodo que permite insertar datos en la lista ordenada*/
void GestorEstructuras::insertarListaOrdenada(const std::string &nombre,const std::string &key,const std::string &categoria, const std::string &fecha, const std::string &marca, double precio, int stock){

    if (this->existeProductoListaOrdenada(key)) {
        throw InsertException("El Codigo de Barra { " + key + " } ya existe para el producto: "+ nombre +" [LISTA ORDENADA].");
    }

    this->listaOrdenada->insertarAtras(Producto(nombre,key,categoria,fecha,marca,precio,stock));
}


/*Metodo de validacion del csv delegado para validar*/
void GestorEstructuras::validarCsv(const QStringList& columnas, int fila){

    if (columnas.size() != 7) {
        this->listaErrores->insertarAtras(ErroresLectura("Numero de columnas incorrecto (esperadas 7).",fila));
        throw ReaderCsvException("Fila " + std::to_string(fila) +
                                 ": Numero de columnas incorrecto (esperadas 7).");
    }

    for (int i = 0; i < columnas.size(); ++i) {
        if (columnas[i].trimmed().isEmpty()) {
            this->listaErrores->insertarAtras(ErroresLectura("El campo en la columna " + std::to_string(i+1) + " esta vacio.",fila));
            throw ReaderCsvException("Fila " + std::to_string(fila) +
                                     ": El campo en la columna " + std::to_string(i+1) + " esta vacio.");
        }
    }

    bool okPrecio, okStock;
    columnas[5].toDouble(&okPrecio);
    columnas[6].toInt(&okStock);

    if (!okPrecio) {
         this->listaErrores->insertarAtras(ErroresLectura("El Precio no es un numero valido.",fila));
        throw ReaderCsvException("Fila " + std::to_string(fila) + ": El Precio no es un numero valido.");
    }
    if (!okStock) {
        this->listaErrores->insertarAtras(ErroresLectura("El Stock no es un entero valido.",fila));
        throw ReaderCsvException("Fila " + std::to_string(fila) + ": El Stock no es un entero valido.");
    }

    if (!esFechaISO(columnas[3])) {
        this->listaErrores->insertarAtras(ErroresLectura(": La fecha de expiracion: '" + columnas[3].toStdString() + "'. no tiene formato ISO (YYYY-MM-DD).",fila));
        throw ReaderCsvException("Fila " + std::to_string(fila) +
                                 ": La fecha de expiracion: '" + columnas[3].toStdString() + "'. no tiene formato ISO (YYYY-MM-DD).");
    }

}

/*Metodo auxiliar que permite validar el archivo de entrada*/
bool GestorEstructuras::esFechaISO(const QString& fecha) {
    return QDate::fromString(fecha, Qt::ISODate).isValid();
}

/*Metodo que permite validar las entradas antes de insertar*/
void GestorEstructuras::validarInsercion(const std::string &_nombre, const std::string &_codigoBarra,
                                    const std::string &_categoria, const std::string &_fechaExpiracion,
                                    const std::string &_marca, const std::string &_precio,
                                    const std::string &_stock)
{

    QString nombre = QString::fromStdString(_nombre).trimmed();
    QString barra = QString::fromStdString(_codigoBarra).trimmed();
    QString categoria = QString::fromStdString(_categoria).trimmed();
    QString fecha = QString::fromStdString(_fechaExpiracion).trimmed();
    QString marca = QString::fromStdString(_marca).trimmed();
    QString precioStr = QString::fromStdString(_precio).trimmed();
    QString stockStr = QString::fromStdString(_stock).trimmed();

    if (nombre.isEmpty() || barra.isEmpty() || categoria.isEmpty() ||
        fecha.isEmpty() || marca.isEmpty() || precioStr.isEmpty() || stockStr.isEmpty()) {
        throw InsertException("Error: Todos los campos son obligatorios.");
    }

    bool okPrecio;
    double precioVal = precioStr.toDouble(&okPrecio);
    if (!okPrecio || precioVal < 0) {
        throw InsertException("Error: El precio '" + _precio + "' no es un numero valido o es negativo.");
    }

    bool okStock;
    int stockVal = stockStr.toInt(&okStock);
    if (!okStock || stockVal < 0) {
        throw InsertException("Error: El stock '" + _stock + "' no es un entero valido o es negativo.");
    }

    QDate checkFecha = QDate::fromString(fecha, "yyyy-MM-dd");
    if (!checkFecha.isValid()) {
        throw InsertException("Error: La fecha '" + _fechaExpiracion + "' no tiene formato ISO valido (YYYY-MM-DD).");
    }

}



/*Metodo que permite armar el log de errores*/
QString GestorEstructuras::generarContenidoLog() {
    QString log;
    QDateTime current = QDateTime::currentDateTime();
    QString fechaStr = current.toString("yyyy-MM-dd hh:mm:ss");

    log += "--------------------------------------------------\n";
    log += "LOG DE ERRORES --- PROCESAMIENTO CSV--- \n";
    log += "Fecha: " + fechaStr + "\n";
    log += "--------------------------------------------------\n\n";

    log += "--------------------------------------------------\n";
    log += "--- SISTEMA DE GESTION DE PRODUCTOS--- \n";
    log += "--------------------------------------------------\n\n";

    for (int i = 0; i < this->listaErrores->getLongitud(); ++i) {

        ErroresLectura erroresLectura = this->listaErrores->getValor(i);
        log += "[" + fechaStr + "] [ERROR] ";

        log += "Fila " + QString::number(erroresLectura.getLinea()) + ": ";
        log += QString::fromStdString(erroresLectura.getMensaje()) + "\n";
    }

    if (this->listaErrores->getLongitud() == 0) {
        log += "No se reportaron errores en la ultima carga.\n";
    }

    return log;
}


/*Apartado de Metodos utilizados para poder crear la lista ordenada y ordenar las listas acorde a los diferentes parametros*/

/*Metodo delegado para poder ordenar a la lista en base a un criterio*/
/*
* 1 -> nombre
* 2 -> categoria
* 3 -> fecha
*/
void GestorEstructuras::generarListaOrdenada(int criterio){

    if(this->listaOrdenada != nullptr){
        delete this->listaOrdenada;
        this->listaOrdenada = nullptr;
    }
    this->listaOrdenada = new ListaEnlazada<Producto>();

    std::vector<Producto> buffer;
    NodoLista<Producto>* actual = this->listaNoOrdenada->getCabeza();
    while (actual != nullptr) {
        buffer.push_back(actual->getDato());
        actual = actual->getSiguiente();
    }

    int n = buffer.size();

    for (int i = 0; i < n - 1; i++) {
        int indiceMinimo = i;
        for (int j = i + 1; j < n; j++) {

            bool esMenor = false;
            if (criterio == 1){
                esMenor = (buffer[j].getNombre() < buffer[indiceMinimo].getNombre());
            }
            else if (criterio == 2){
                esMenor = (buffer[j].getCategoria() < buffer[indiceMinimo].getCategoria());
            }
            else if (criterio == 3){
                esMenor = (buffer[j].getFechaExpiracion() < buffer[indiceMinimo].getFechaExpiracion());
            }
            if (esMenor) {
                indiceMinimo = j;
            }
        }

        /*Clasico*/
        Producto temp = buffer[i];
        buffer[i] = buffer[indiceMinimo];
        buffer[indiceMinimo] = temp;
    }

    for (const Producto& p : buffer) {
        this->listaOrdenada->insertarAtras(p);
    }

}


/*Metodo delegado para poder ordenar a la lista en base a un criterio*/
/*
* 1 -> nombre
* 2 -> categoria
* 3 -> fecha
*/
void GestorEstructuras::ordenarLista(int criterio){

    if (this->listaOrdenada == nullptr || this->listaOrdenada->getLongitud() <= 1) {
        return;
    }

    std::vector<Producto> buffer;
    NodoLista<Producto>* actual = this->listaOrdenada->getCabeza();
    while (actual != nullptr) {
        buffer.push_back(actual->getDato());
        actual = actual->getSiguiente();
    }

    int n = buffer.size();

    for (int i = 0; i < n - 1; i++) {
        int indiceMinimo = i;
        for (int j = i + 1; j < n; j++) {

            bool esMenor = false;
            if (criterio == 1){
                esMenor = (buffer[j].getNombre() < buffer[indiceMinimo].getNombre());
            }
            else if (criterio == 2){
                esMenor = (buffer[j].getCategoria() < buffer[indiceMinimo].getCategoria());
            }
            else if (criterio == 3){
                esMenor = (buffer[j].getFechaExpiracion() < buffer[indiceMinimo].getFechaExpiracion());
            }
            if (esMenor) {
                indiceMinimo = j;
            }
        }

        /*Clasico*/
        Producto temp = buffer[i];
        buffer[i] = buffer[indiceMinimo];
        buffer[indiceMinimo] = temp;
    }

    this->listaOrdenada->limpiar();

    for (const Producto& p : buffer) {
        this->listaOrdenada->insertarAtras(p);
    }
}
/*Fin del Apartado de Metodos utilizados para poder ordenar las listas acorde a los diferentes parametros*/


/*-*----Metodos de busqueda por nombre----*--*/

/*Busqueda en arbol AVL*/
ListaEnlazada<Producto> GestorEstructuras::buscarProductoAvl(const std::string& nombre){
    return this->arbolAvl->buscarProducto(nombre);
}

/*Busqueda en lista ordenada*/
ListaEnlazada<Producto> GestorEstructuras::buscarProductoListaOrdenada(const std::string& nombre){

    ListaEnlazada<Producto> resultados;

    int longitud = this->listaOrdenada->getLongitud();

    for (int i = 0; i < longitud; i++) {
        Producto product = this->listaOrdenada->getValor(i);

        if (product.getNombre() == nombre) {
            resultados.insertarAtras(product);
        }
        else if (product.getNombre() > nombre) {
            break;
        }
    }

    return resultados;

}

/*Busqueda en lista no ordenada*/
ListaEnlazada<Producto> GestorEstructuras::buscarProductoListaNoOrdenada(const std::string& nombre){

    ListaEnlazada<Producto> resultados;

    int longitud = this->listaNoOrdenada->getLongitud();

    for (int i = 0; i < longitud; i++) {
        Producto product = this->listaNoOrdenada->getValor(i);

        if (product.getNombre() == nombre) {
            resultados.insertarAtras(product);
        }
    }

    return resultados;
}


/*-*----Metodos de busqueda por categoria----*--*/

/*Busqueda en arbol AVL*/
ListaEnlazada<Producto> GestorEstructuras::buscarProductoBMas(const std::string& categoria){
    return this->arbolBMas->buscarPorCategoria(categoria);
}

/*Busqueda en lista ordenada*/
ListaEnlazada<Producto> GestorEstructuras::buscarProductoCategoriaListaOrdenada(const std::string& categoria){

    ListaEnlazada<Producto> resultados;

    int longitud = this->listaOrdenada->getLongitud();

    for (int i = 0; i < longitud; i++) {
        Producto product = this->listaOrdenada->getValor(i);

        if (product.getCategoria() == categoria) {
            resultados.insertarAtras(product);
        }
        else if (product.getCategoria() > categoria) {
            break;
        }
    }

    return resultados;

}

/*Busqueda en lista no ordenada*/
ListaEnlazada<Producto> GestorEstructuras::buscarProductoCategoriaListaNoOrdenada(const std::string& categoria){

    ListaEnlazada<Producto> resultados;

    int longitud = this->listaNoOrdenada->getLongitud();

    for (int i = 0; i < longitud; i++) {
        Producto product = this->listaNoOrdenada->getValor(i);

        if (product.getCategoria() == categoria) {
            resultados.insertarAtras(product);
        }
    }

    return resultados;
}



