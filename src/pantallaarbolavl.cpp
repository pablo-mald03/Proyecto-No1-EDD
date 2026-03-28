#include "pantallaarbolavl.h"
#include "ui_pantallaarbolavl.h"
#include"graphicsviewzoom.h"

#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

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
int PantallaArbolAvl::dibujarNodo(int x, int y, int valor) {

    int padding = 10;

    QGraphicsTextItem* text = scene->addText(QString::number(valor));
    QRectF rect = text->boundingRect();

    int ancho = rect.width() + padding * 2;
    int alto  = rect.height() + padding * 2;

    int size = std::max(ancho, alto);

    this->scene->addEllipse(x, y, size, size, QPen(Qt::white));

    int textX = x + (size - rect.width()) / 2;
    int textY = y + (size - rect.height()) / 2;

    text->setPos(textX, textY);

    return size;
}

/*Metodo que permite dibujar las lineas de trazo*/
void PantallaArbolAvl::dibujarLinea(int x1, int y1, int x2, int y2) {
    QPen pen(Qt::white);
    pen.setWidth(1);
    scene->addLine(x1, y1, x2, y2, pen);
}

void PantallaArbolAvl::dibujarArbol(NodoFake* nodo, int x, int y, int offset) {

    if (!nodo){
        return;
    }

    int size = dibujarNodo(x, y, nodo->valor);

    int centerX = x + size / 2;
    int bottomY = y + size;

    if (nodo->izq) {

        int childX = x - offset;
        int childY = y + 80;

        dibujarLinea(
            centerX,
            bottomY,
            childX + size / 2,
            childY
            );

        dibujarArbol(nodo->izq, childX, childY, offset / 2);
    }

    if (nodo->der) {

        int childX = x + offset;
        int childY = y + 80;

        dibujarLinea(
            centerX,
            bottomY,
            childX + size / 2,
            childY
            );

        dibujarArbol(nodo->der, childX, childY, offset / 2);
    }
}

/*Metodo que permite exportar el .dot*/
void PantallaArbolAvl::on_btnExportar_clicked()
{

    QString ruta = QFileDialog::getSaveFileName(
        this,
        "Guardar arbol B como imagen",
        "",
        "Imagen PNG (*.png)"
        );

    if (ruta.isEmpty()) return;

    QString dotPath = QDir::tempPath() + "/arbol_avl.dot";

    QFile file(dotPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo crear el archivo temporal.");
        return;
    }

    QTextStream out(&file);

    /*CODIGO HARDOCDEADO DE MOMENTO. A LA ESPERA DEL CODIGO REAL PARA PODER GENERAR EL GRAPHVIZ RESPECTIVO*/
    out << "digraph G {\n";

    out << "node [shape=circle, style=filled, fillcolor=lightblue];\n";

    // nodos
    out << "n10 [label=\"10\"];\n";
    out << "n5  [label=\"5\"];\n";
    out << "n15 [label=\"15\"];\n";
    out << "n3  [label=\"3\"];\n";
    out << "n7  [label=\"7\"];\n";

    // conexiones
    out << "n10 -> n5;\n";
    out << "n10 -> n15;\n";
    out << "n5 -> n3;\n";
    out << "n5 -> n7;\n";

    out << "}\n";

    file.close();


#ifdef Q_OS_WIN
    QString programa = "C:/Program Files/Graphviz/bin/dot.exe";
#else
    QString programa = "dot";
#endif

    QProcess proceso;
    QStringList argumentos;
    argumentos << "-Tpng" << dotPath << "-o" << ruta;

    proceso.start(programa, argumentos);

    if (!proceso.waitForFinished()) {
        QMessageBox::critical(this, "Error", "No se pudo ejecutar Graphviz.");
        return;
    }

    if (proceso.exitStatus() == QProcess::NormalExit && proceso.exitCode() == 0) {

        QMessageBox::information(this, "Exito", "Imagen generada correctamente.");
        QDesktopServices::openUrl(QUrl::fromLocalFile(ruta));

    } else {

        QMessageBox::critical(this, "Error", "Fallo la generacion de la imagen.");
    }

}

