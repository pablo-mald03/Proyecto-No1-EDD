#include "pantallaverarboles.h"
#include "ui_pantallaverarboles.h"

PantallaVerArboles::PantallaVerArboles(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaVerArboles)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaVerArboles { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");
}

PantallaVerArboles::~PantallaVerArboles()
{
    delete ui;
}
