#include "pantallaarbolb.h"
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
void PantallaArbolB::setArbol(int * _arbol){
    this->arbol = _arbol;
    actualizarVista();
}

/*Metodo que permite actualizar la vista*/
void PantallaArbolB::actualizarVista(){

    this->scene->clear();

    /*pendiente integracion real*/
    NodoBFake* raiz = crearArbolBPrueba();

    dibujarArbolB(raiz, 0, 0, 150);

    QRectF bounds = scene->itemsBoundingRect();
    scene->setSceneRect(bounds.adjusted(-200, -200, 200, 200));

}


/*METODO QUEMADO. PENDIENTE LA INTEGRACION REAL (SERA REMOVIDO)*/
NodoBFake* PantallaArbolB::crearArbolBPrueba() {
    NodoBFake* raiz = new NodoBFake{{10, 20}, {}};

    NodoBFake* hijo1 = new NodoBFake{{5, 8}, {}};
    NodoBFake* hijo2 = new NodoBFake{{12, 15}, {}};
    NodoBFake* hijo3 = new NodoBFake{{25, 30}, {}};

    raiz->hijos = {hijo1, hijo2, hijo3};

    return raiz;
}

/*Metodo que permite dibujar a los nodos*/
int PantallaArbolB::dibujarNodoB(int x, int y, NodoBFake* nodo) {

    int paddingX = 10;
    int paddingY = 6;
    int separacion = 5;

    int currentX = x;
    int alturaMax = 0;

    std::vector<QGraphicsTextItem*> textos;

    for (int valor : nodo->claves) {

        QGraphicsTextItem* text = scene->addText(QString::number(valor));
        QRectF rect = text->boundingRect();

        int ancho = rect.width() + paddingX * 2;
        int alto = rect.height() + paddingY * 2;

        alturaMax = std::max(alturaMax, alto);

        text->setPos(currentX + paddingX, y + paddingY);

        textos.push_back(text);

        currentX += ancho + separacion;
    }

    int anchoTotal = currentX - x - separacion;

    scene->addRect(x, y, anchoTotal, alturaMax, QPen(Qt::white));

    currentX = x;

    for (int i = 0; i < textos.size(); i++) {

        QRectF rect = textos[i]->boundingRect();
        int ancho = rect.width() + paddingX * 2;

        if (i > 0) {
            scene->addLine(currentX, y, currentX, y + alturaMax, QPen(Qt::white));
        }

        currentX += ancho + separacion;
    }

    return anchoTotal;
}

/*Metodo que permite dibujar las lineas*/
void PantallaArbolB::dibujarLineaB(int x1, int y1, int x2, int y2) {
    QPen pen(Qt::white);
    pen.setWidth(1);
    this->scene->addLine(x1, y1, x2, y2, pen);
}

void PantallaArbolB::dibujarArbolB(NodoBFake* nodo, int x, int y, int offset) {

    if (!nodo) return;

    int anchoCelda = 30;
    int anchoNodo = dibujarNodoB(x, y, nodo);

    int numHijos = nodo->hijos.size();

    for (int i = 0; i < numHijos; i++) {

        int childX = x - (numHijos - 1) * offset / 2 + i * offset;
        int childY = y + 80;

        dibujarLineaB(
            x + anchoNodo / 2,
            y + 40,
            childX + 20,
            childY
            );

        dibujarArbolB(nodo->hijos[i], childX, childY, offset / 1.5);
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

