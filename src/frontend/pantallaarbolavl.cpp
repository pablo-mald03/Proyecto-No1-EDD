#include "pantallaarbolavl.h"


/*Includes de la clase*/
#include "nodoavl.h"
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
#include<QGraphicsView>
#include <QGraphicsTextItem>

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
    delete ui;
}

/*Metodo delegado para poder cargar la vista al momento de moverse*/
void PantallaArbolAvl::setArbol(NodoAvl * _arbol){
    this->arbol = _arbol;
    actualizarVista();
}

/*Metodo que permite poder actualizar la vista del arbol*/
void PantallaArbolAvl::actualizarVista(){

    this->scene->clear();

    if (this->arbol != nullptr) {
        int alturaTotal = this->arbol->getAltura();


        int offsetInicial = alturaTotal * 120;

        dibujarArbol(this->arbol, 0, 0, offsetInicial);
    }

    scene->setSceneRect(scene->itemsBoundingRect().adjusted(-500, -200, 500, 500));
}

/*Metodo utilizado para poder dibujar a la esecena*/
QRectF PantallaArbolAvl::dibujarNodo(int x, int y, QString texto) {

    int paddingH = 20;
    int paddingV = 10;

    if (texto.length() > 25) {
        texto = texto.left(22) + "...";
    }

    QGraphicsTextItem* text = scene->addText(texto);

    text->setDefaultTextColor(Qt::white);
    text->setZValue(10);

    QFont font = text->font();
    font.setPointSize(10);
    font.setBold(true);
    text->setFont(font);

    QRectF textRect = text->boundingRect();
    int ancho = std::max((int)textRect.width() + paddingH * 2, 160);
    int alto  = textRect.height() + paddingV * 2;
    this->scene->addEllipse(x, y, ancho, alto,
                            QPen(Qt::white, 2),
                            QBrush(QColor(30, 30, 35)));

    text->setPos(x + (ancho - textRect.width()) / 2, y + (alto - textRect.height()) / 2);

    return QRectF(x, y, ancho, alto);
}

/*Metodo que permite dibujar las lineas de trazo*/
void PantallaArbolAvl::dibujarLinea(int x1, int y1, int x2, int y2) {
    QPen pen(Qt::white);
    pen.setWidth(1);
    scene->addLine(x1, y1, x2, y2, pen);
}

void PantallaArbolAvl::dibujarArbol(NodoAvl* nodo, int x, int y, int offset) {

    if (!nodo){
        return;
    }

    QString nombre = QString::fromStdString(nodo->getDato().getNombre());
    QRectF nodoRect = dibujarNodo(x, y, nombre);

    int centerX = x + nodoRect.width() / 2;
    int bottomY = y + nodoRect.height();

    int distanciaVertical = 180;

    int nuevoOffset = offset * 0.8;

    if (nuevoOffset < 100) nuevoOffset = 100;

    if (nodo->getIzquierda()) {
        int childX = centerX - nuevoOffset - nodoRect.width() / 2;
        int childY = y + distanciaVertical;

        dibujarLinea(centerX, bottomY, childX + nodoRect.width() / 2, childY);
        dibujarArbol(nodo->getIzquierda(), childX, childY, nuevoOffset);
    }

    if (nodo->getDerecha()) {
        int childX = centerX + nuevoOffset - nodoRect.width() / 2;
        int childY = y + distanciaVertical;

        dibujarLinea(centerX, bottomY, childX + nodoRect.width() / 2, childY);
        dibujarArbol(nodo->getDerecha(), childX, childY, nuevoOffset);
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

