#include "pantallabuscarrango.h"
#include "ui_pantallabuscarrango.h"

PantallaBuscarRango::PantallaBuscarRango(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaBuscarRango)
{
    ui->setupUi(this);
}

PantallaBuscarRango::~PantallaBuscarRango()
{
    delete ui;
}
