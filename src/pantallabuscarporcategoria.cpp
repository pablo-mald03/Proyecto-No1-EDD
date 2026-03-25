#include "pantallabuscarporcategoria.h"
#include "ui_pantallabuscarporcategoria.h"

PantallaBuscarPorCategoria::PantallaBuscarPorCategoria(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaBuscarPorCategoria)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaBuscarPorCategoria { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");
}

PantallaBuscarPorCategoria::~PantallaBuscarPorCategoria()
{
    delete ui;
}
