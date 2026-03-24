#include "pantallaeliminarproducto.h"
#include "ui_pantallaeliminarproducto.h"

PantallaEliminarProducto::PantallaEliminarProducto(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaEliminarProducto)
{
    ui->setupUi(this);
}

PantallaEliminarProducto::~PantallaEliminarProducto()
{
    delete ui;
}
