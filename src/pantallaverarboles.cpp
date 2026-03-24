#include "pantallaverarboles.h"
#include "ui_pantallaverarboles.h"

PantallaVerArboles::PantallaVerArboles(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaVerArboles)
{
    ui->setupUi(this);
}

PantallaVerArboles::~PantallaVerArboles()
{
    delete ui;
}
