#include "pantallabuscarrango.h"
#include "ui_pantallabuscarrango.h"

PantallaBuscarRango::PantallaBuscarRango(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaBuscarRango)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaBuscarRango { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");
}

PantallaBuscarRango::~PantallaBuscarRango()
{
    delete ui;
}
