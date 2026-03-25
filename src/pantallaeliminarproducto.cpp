#include "pantallaeliminarproducto.h"
#include "ui_pantallaeliminarproducto.h"

PantallaEliminarProducto::PantallaEliminarProducto(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaEliminarProducto)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaEliminarProducto { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");
}

PantallaEliminarProducto::~PantallaEliminarProducto()
{
    delete ui;
}
