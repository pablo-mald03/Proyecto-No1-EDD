#include "pantallabuscarporcategoria.h"
#include "ui_pantallabuscarporcategoria.h"

PantallaBuscarPorCategoria::PantallaBuscarPorCategoria(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaBuscarPorCategoria)
{
    ui->setupUi(this);
}

PantallaBuscarPorCategoria::~PantallaBuscarPorCategoria()
{
    delete ui;
}
