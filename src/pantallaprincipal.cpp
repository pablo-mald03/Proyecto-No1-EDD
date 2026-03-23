#include "pantallaprincipal.h"
#include "ui/ui_pantallaprincipal.h"

PantallaPrincipal::PantallaPrincipal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaPrincipal)
{
    ui->setupUi(this);


    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaPrincipal { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");
}

PantallaPrincipal::~PantallaPrincipal()
{
    delete ui;
}
