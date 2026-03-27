#include "pantallaarbolavl.h"
#include "ui_pantallaarbolavl.h"
#include"graphicsviewzoom.h"

PantallaArbolAvl::PantallaArbolAvl(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaArbolAvl)
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
PantallaArbolAvl::~PantallaArbolAvl()
{
   // delete raiz;
    delete ui;
}

/*Metodo delegado para poder cargar la vista al momento de moverse*/

void PantallaArbolAvl::setArbol(int * _arbol){
    this->arbol = _arbol;
    actualizarVista();
}

/*Metodo que permite poder actualizar la vista del arbol*/

void PantallaArbolAvl::actualizarVista(){

    this->scene->clear();

    /*pendiente integracion real*/
    /*NodoAvl* raiz = this.arbol*/

    raiz = crearArbolPrueba();

    dibujarArbol(raiz, 0, 0, 120);

    QRectF bounds = scene->itemsBoundingRect();
    scene->setSceneRect(bounds.adjusted(-200, -200, 200, 200));

}

/*ARBOL SIMULADO (SERA REMOVIDO CUANDO SE INTEGRE EL BACKEND)*/
NodoFake* PantallaArbolAvl::crearArbolPrueba() {
    NodoFake* raiz = new NodoFake{10, nullptr, nullptr};
    raiz->izq = new NodoFake{5, nullptr, nullptr};
    raiz->der = new NodoFake{15, nullptr, nullptr};
    raiz->izq->izq = new NodoFake{3, nullptr, nullptr};
    raiz->izq->der = new NodoFake{7, nullptr, nullptr};
    return raiz;
}

/*Metodo utilizado para poder dibujar a la esecena*/
void PantallaArbolAvl::dibujarNodo(int x, int y, int valor) {
    int radio = 20;

    scene->addEllipse(x, y, radio*2, radio*2, QPen(Qt::white));
    QGraphicsTextItem* text = this->scene->addText(QString::number(valor));
    text->setPos(x + 10, y + 5);
}

/*Metodo que permite dibujar las lineas de trazo*/
void PantallaArbolAvl::dibujarLinea(int x1, int y1, int x2, int y2) {
    QPen pen(Qt::white);
    pen.setWidth(1);
    scene->addLine(x1, y1, x2, y2, pen);
}

void PantallaArbolAvl::dibujarArbol(NodoFake* nodo, int x, int y, int offset) {
    if (!nodo) return;

    dibujarNodo(x, y, nodo->valor);

    if (nodo->izq) {
        dibujarLinea(x + 20, y + 40, x - offset + 20, y + 80);
        dibujarArbol(nodo->izq, x - offset, y + 80, offset / 2);
    }

    if (nodo->der) {
        dibujarLinea(x + 20, y + 40, x + offset + 20, y + 80);
        dibujarArbol(nodo->der, x + offset, y + 80, offset / 2);
    }
}

