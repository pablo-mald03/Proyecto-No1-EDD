#include "pantallaarbolb.h"
#include "ui_pantallaarbolb.h"

PantallaArbolB::PantallaArbolB(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaArbolB)
{
    ui->setupUi(this);
}

PantallaArbolB::~PantallaArbolB()
{
    delete ui;
}
