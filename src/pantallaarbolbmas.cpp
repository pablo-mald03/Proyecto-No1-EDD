#include"pantallaarbolbmas.h"
#include "ui_pantallaarbolbmas.h"


PantallaArbolBMas::PantallaArbolBMas(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaArbolBMas)
{
    ui->setupUi(this);
}

PantallaArbolBMas::~PantallaArbolBMas()
{
    delete ui;
}
