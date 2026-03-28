#ifndef PANTALLAARBOLB_H
#define PANTALLAARBOLB_H

#include <QWidget>
#include<QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsTextItem>


/*STRUCT QUEMADO NO TIENE NINGUN CONTEXTO EN LA FASE DE FRONT MAS QUE ESTAR QUEMADO*/
struct NodoBFake {
    std::vector<int> claves;
    std::vector<NodoBFake*> hijos;
};

namespace Ui {
class PantallaArbolB;
}

class PantallaArbolB : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaArbolB(QWidget *parent = nullptr);
    ~PantallaArbolB();
    void setArbol(int * _arbol);

signals:
    void solicitarArbolB();

private slots:
    void on_btnExportar_clicked();

private:
    Ui::PantallaArbolB *ui;

    QGraphicsScene * scene = nullptr;

    /*Referencia del arbol*/
    int * arbol = nullptr;

    /*Metodo que permite dibujar a los nodos*/
    int dibujarNodoB(int x, int y, NodoBFake* nodo);

    /*Metodo que permite dibujar a las lineas que unen a los nodos*/
    void dibujarLineaB(int x1, int y1, int x2, int y2);

    /*Pendiente reemplazar PENDIENTE LA INTEGRACION REAL*/
    void dibujarArbolB(NodoBFake* nodo, int x, int y, int offset);

    /*METODO TEMPORAL*/
    NodoBFake* crearArbolBPrueba();

    /*Metodo que actualiza la vista*/
    void actualizarVista();
};

#endif // PANTALLAARBOLB_H
