#include "pantallalistarnombre.h"
#include "ui_pantallalistarnombre.h"

PantallaListarNombre::PantallaListarNombre(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaListarNombre)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaListarNombre { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");
}

PantallaListarNombre::~PantallaListarNombre()
{
    delete ui;
}
