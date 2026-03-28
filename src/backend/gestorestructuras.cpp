#include "gestorestructuras.h"
#include "readercsvexception.h"

GestorEstructuras::GestorEstructuras():
    listaNoOrdenada(new ListaEnlazada<Producto>()),
    listaOrdenada(new ListaEnlazada<Producto>()),
    listaErrores(new ListaEnlazada<ErroresLectura>())
{

}

/*Destructor*/
GestorEstructuras::~GestorEstructuras(){
    if(this->listaNoOrdenada != nullptr){
        delete this->listaNoOrdenada;
    }

    if(this->listaOrdenada != nullptr){
        delete this->listaOrdenada;
    }

    if(this->listaErrores != nullptr){
        delete this->listaErrores;
    }
}


/*----****------Apartado de metodos setter para poder interactuar con las listas------****---*/

/*Metodo de validacion del csv delegado para validar*/
void GestorEstructuras::agregarErrorLista(const std::string mensaje, int fila){

    this->listaErrores->insertarAtras(ErroresLectura(mensaje, fila));
}

/*----****------Fin del Apartado de metodos setter para poder interactuar con las listas------****---*/


/*Metodo que permite insertar los datos en la lista*/
void GestorEstructuras::insertarListas(std::string nombre, std::string key, std::string categoria, std::string fecha, std::string marca, double precio, int stock){

    this->listaNoOrdenada->insertarAtras(Producto(nombre,key,categoria,fecha,marca,precio,stock));

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
