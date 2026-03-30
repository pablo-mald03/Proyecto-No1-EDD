#include "pantallaarbolb.h"

/*Includes de la clase*/
#include"nodob.h"
#include "graphicsviewzoom.h"
#include "ui_pantallaarbolb.h"

#include <QFileDialog>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

PantallaArbolB::PantallaArbolB(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaArbolB)
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
PantallaArbolB::~PantallaArbolB()
{
    delete ui;
}

/*Metodo delegado para poder cargar la vista al momento de moverse*/
void PantallaArbolB::setArbol(NodoB * _arbol){
    this->raiz = _arbol;
    actualizarVista();
}

/*Metodo que permite actualizar la vista*/
void PantallaArbolB::actualizarVista(){

    this->scene->clear();

    if (this->raiz != nullptr) {
        dibujarArbolB(this->raiz, 0, 0, 400);
    }

    QRectF bounds = scene->itemsBoundingRect();
    scene->setSceneRect(bounds.adjusted(-200, -200, 200, 200));
}


/*Metodo que permite dibujar a los nodos*/
int PantallaArbolB::dibujarNodoB(int x, int y, NodoB* nodo) {

    int paddingX = 12;
    int paddingY = 8;
    int currentX = x;
    int alturaMax = 0;

    std::vector<QGraphicsTextItem*> textos;

    int nClaves = nodo->getClaves().getLongitud();

    for (int i = 0; i < nClaves; i++) {
        Producto product = nodo->getClaves().getValor(i);
        QString etiqueta = QString::fromStdString(product.getFechaExpiracion());

        QGraphicsTextItem* text = scene->addText(etiqueta);
        text->setDefaultTextColor(Qt::white);

        QRectF rect = text->boundingRect();
        int ancho = rect.width() + paddingX * 2;
        int alto = rect.height() + paddingY * 2;

        alturaMax = std::max(alturaMax, alto);
        text->setPos(currentX + paddingX, y + paddingY);
        textos.push_back(text);

        currentX += ancho;
    }

    int anchoTotal = currentX - x;

    scene->addRect(x, y, anchoTotal, alturaMax, QPen(Qt::white), QBrush(QColor(219, 234, 254)));

    currentX = x;
    for (int i = 0; i < textos.size(); i++) {
        QRectF rect = textos[i]->boundingRect();
        int ancho = rect.width() + paddingX * 2;

        if (i < textos.size() - 1) {
            scene->addLine(currentX + ancho, y, currentX + ancho, y + alturaMax, QPen(Qt::black));
        }
        currentX += ancho;
        textos[i]->setDefaultTextColor(Qt::black);
    }

    return anchoTotal;
}

/*Metodo que permite dibujar las lineas*/
void PantallaArbolB::dibujarLineaB(int x1, int y1, int x2, int y2) {
    QPen pen(Qt::white);
    pen.setWidth(1);
    this->scene->addLine(x1, y1, x2, y2, pen);
}

/*Metodo que permite dibujar el arbol B*/
void PantallaArbolB::dibujarArbolB(NodoB* nodo, int x, int y, int offset) {

    if (!nodo){
        return;
    }

    int anchoNodo = dibujarNodoB(x, y, nodo);

    if (nodo->getEsHoja()) return;

    int numHijos = nodo->getHijos().getLongitud();

    int inicioX = x + (anchoNodo / 2) - ((numHijos - 1) * offset / 2);

    for (int i = 0; i < numHijos; i++) {
        NodoB* hijo = nodo->getHijos().getValor(i);
        if (!hijo) continue;

        int childX = inicioX + (i * offset);
        int childY = y + 100;

        dibujarLineaB(
            x + (anchoNodo / numHijos) * i + (anchoNodo / (numHijos * 2)),
            y + 30,
            childX + 20,
            childY
            );

        dibujarArbolB(hijo, childX, childY, offset / 1.8);
    }
}



/*Metodo que permite exportar el arbol B*/
void PantallaArbolB::on_btnExportar_clicked()
{

    QString ruta = QFileDialog::getSaveFileName(
        this,
        "Guardar arbol B como imagen",
        "",
        "Imagen PNG (*.png)"
        );

    if (ruta.isEmpty()) return;

    QString dotPath = QDir::tempPath() + "/arbol_b.dot";

    QFile file(dotPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo crear el archivo temporal.");
        return;
    }

    QTextStream out(&file);

    /*CODIGO HARDCODEADO TEMPORAL YA QUE SE SUSTITUIRA POR LA INTEGRACION REAL DE LOS ARBOLES*/
    out << "digraph G {\n";
    out << "node [shape=record, style=filled, fillcolor=\"#dbeafe\", color=\"black\"];\n";

    // Nodo raíz con 2 claves  3 hijos (SINTAXIS DEJADA PARA MI PORQUE NO ME ACUERDO DEL TODO DE LAS REGLAS)
    out << "n1 [label=\"<f0> |10| <f1> |20| <f2>\"];\n";

    // Hijos
    out << "n2 [label=\"5|8\"];\n";
    out << "n3 [label=\"12|15\"];\n";
    out << "n4 [label=\"25|30\"];\n";

    // Conexiones
    out << "n1:f0 -> n2;\n";
    out << "n1:f1 -> n3;\n";
    out << "n1:f2 -> n4;\n";

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

