#include"pantallaarbolbmas.h"
#include "ui_pantallaarbolbmas.h"
#include "graphicsviewzoom.h"


PantallaArbolBMas::PantallaArbolBMas(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaArbolBMas)
{
    ui->setupUi(this);

    this->scene = new QGraphicsScene(this);

    auto view = new GraphicsViewZoom(this);
    view->setScene(scene);

    this->ui->verticalLayout->replaceWidget(ui->graphicsView, view);

    delete ui->graphicsView;

    this->ui->graphicsView = view;
}

/*Destructor*/
PantallaArbolBMas::~PantallaArbolBMas()
{
    delete ui;
}

/*Metodo delegado para poder cargar la vista al momento de moverse*/
void PantallaArbolBMas::setArbol(int * _arbol){
    this->arbol = _arbol;
    actualizarVista();
}

/*Metodo que permite actualizar la vista*/
void PantallaArbolBMas::actualizarVista(){

    this->scene->clear();

    /*pendiente integracion real*/
    /*NodoAvl* raiz = this.arbol*/

    /* raiz = crearArbolPrueba();

    dibujarArbol(raiz, 0, 0, 120);*/

    QRectF bounds = scene->itemsBoundingRect();
    scene->setSceneRect(bounds.adjusted(-200, -200, 200, 200));

}

/*Metodo que permite exportar el arbol B+*/
void PantallaArbolBMas::on_btnExportar_clicked()
{

}

