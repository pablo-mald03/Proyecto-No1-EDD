#include "pantallaagregar.h"
#include "ui_pantallaagregar.h"

PantallaAgregar::PantallaAgregar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaAgregar)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaAgregar { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");
}

PantallaAgregar::~PantallaAgregar()
{
    delete ui;
}
