#include "pantallabuscarpornombre.h"
#include "ui_pantallabuscarpornombre.h"

PantallaBuscarPorNombre::PantallaBuscarPorNombre(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaBuscarPorNombre)
{
    ui->setupUi(this);
}

PantallaBuscarPorNombre::~PantallaBuscarPorNombre()
{
    delete ui;
}
