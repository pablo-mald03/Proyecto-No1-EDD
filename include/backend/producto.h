#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <string>

class Producto
{
public:
    Producto();
    Producto(const std::string &_nombre,const std::string &_codigoBarra, const std::string &_categoria, const std::string &_fechaExpiracion, const std::string &_marca, double _precio, int _stock);
    ~Producto();

    std::string nombre;
    std::string codigoBarra;
    std::string categoria;
    std::string fechaExpiracion;
    std::string marca;
    double precio;
    int stock;


    /*Metodos getter */

    std::string getNombre();
    std::string getCodigobarra();
    std::string getCategoria();
    std::string getFechaExpiracion();
    std::string getMarca();
    double getPrecio();
    int getStock();

};

#endif // PRODUCTO_H
