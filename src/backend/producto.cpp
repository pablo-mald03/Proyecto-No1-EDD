#include "producto.h"

/*Constructor por defecto*/

Producto::Producto(): nombre(""),
    codigoBarra(""),
    categoria(""),
    fechaExpiracion(""),
    marca(""),
    precio(0.0),
    stock(0)
{}

/*Constructor con parametros*/
Producto::Producto(const std::string &_nombre,const std::string &_codigoBarra, const std::string &_categoria, const std::string &_fechaExpiracion, const std::string &_marca, double _precio, int _stock):
    nombre(_nombre),
    codigoBarra(_codigoBarra),
    categoria(_categoria),
    fechaExpiracion(_fechaExpiracion),
    marca(_marca),
    precio(_precio),
    stock(_stock)
{}

/*Metodos getter de la clase*/
std::string Producto::getNombre(){
    return this->nombre;
}

std::string Producto::getCodigobarra(){
    return this->codigoBarra;
}

std::string Producto::getCategoria(){
    return this->categoria;
}

std::string Producto::getFechaExpiracion(){
    return this->fechaExpiracion;
}

std::string Producto::getMarca(){
    return this->marca;
}

double Producto::getPrecio(){
    return this->precio;
}

int Producto::getStock(){
    return this->stock;
}
