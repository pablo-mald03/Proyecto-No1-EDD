#include "pantallacompararbusqueda.h"
#include "ui_pantallacompararbusqueda.h"

PantallaCompararBusqueda::PantallaCompararBusqueda(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaCompararBusqueda)
{
    ui->setupUi(this);
}

PantallaCompararBusqueda::~PantallaCompararBusqueda()
{
    delete ui;
}
