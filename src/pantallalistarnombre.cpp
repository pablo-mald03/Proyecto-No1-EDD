#include "pantallalistarnombre.h"
#include "ui_pantallalistarnombre.h"

PantallaListarNombre::PantallaListarNombre(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaListarNombre)
{
    ui->setupUi(this);
}

PantallaListarNombre::~PantallaListarNombre()
{
    delete ui;
}
