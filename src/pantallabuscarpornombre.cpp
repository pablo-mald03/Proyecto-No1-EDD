#include "pantallabuscarpornombre.h"
#include "ui_pantallabuscarpornombre.h"

PantallaBuscarPorNombre::PantallaBuscarPorNombre(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaBuscarPorNombre)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaBuscarPorNombre { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");
}

PantallaBuscarPorNombre::~PantallaBuscarPorNombre()
{
    delete ui;
}
