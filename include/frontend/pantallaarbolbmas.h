#ifndef PANTALLAARBOLBMAS_H
#define PANTALLAARBOLBMAS_H

#include <QWidget>
#include<QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsTextItem>

/*STRUCT QUEMADO NO TIENE NINGUN CONTEXTO EN LA FASE DE FRONT MAS QUE ESTAR QUEMADO*/
struct NodoBMasFake {
    std::vector<int> claves;
    std::vector<NodoBMasFake*> hijos;
    bool esHoja;
};

namespace Ui {
class PantallaArbolBMas;
}

class PantallaArbolBMas : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaArbolBMas(QWidget *parent = nullptr);
    ~PantallaArbolBMas();

    void setArbol(int * _arbol);

signals:
    void solicitarArbolBMas();

private slots:
    void on_btnExportar_clicked();

private:
    Ui::PantallaArbolBMas *ui;

    QGraphicsScene * scene = nullptr;

    /*Referencia del arbol*/
    int * arbol = nullptr;

    /*Metodo que actualiza la vista*/
    void actualizarVista();

    /*Metodo que permite dibujar a los nodos*/
    int dibujarNodoBMas(int x, int y, NodoBMasFake* nodo);

    /*Metodo que permite dibujar a las lineas que unen a los nodos*/
    void dibujarLineaBMas(int x1, int y1, int x2, int y2);


    /*Pendiente reemplazar PENDIENTE LA INTEGRACION REAL*/
    void dibujarArbolBMas(NodoBMasFake* nodo, int x, int y, int offset);


    /*METODO QUEMADO. PENDIENTE LA INTEGRACION REAL (SERA REMOVIDO)*/
    NodoBMasFake* crearArbolBMasPrueba();
};

#endif // PANTALLAARBOLBMAS_H
