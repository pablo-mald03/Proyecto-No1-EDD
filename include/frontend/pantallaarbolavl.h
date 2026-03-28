#ifndef PANTALLAARBOLAVL_H
#define PANTALLAARBOLAVL_H

#include <QWidget>
#include<QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsTextItem>

/*ARBOL SIMULADO (SERA REMOVIDO CUANDO SE INTEGRE EL BACKEND)*/
struct NodoFake {
    int valor;
    NodoFake* izq;
    NodoFake* der;
};


namespace Ui {
class PantallaArbolAvl;
}

class PantallaArbolAvl : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaArbolAvl(QWidget *parent = nullptr);
    ~PantallaArbolAvl();

    void setArbol(int * _arbol);

signals:
    void solicitarArbolAvl();

private slots:
    void on_btnExportar_clicked();

private:
    Ui::PantallaArbolAvl *ui;

    QGraphicsScene * scene = nullptr;

    /*TEMPORAL*/
    NodoFake* raiz = nullptr;

    /*Referencia del arbol*/
    int * arbol = nullptr;

    /*Metodo que actualiza la vista*/
    void actualizarVista();

    /*Metodos utilizador para poder dibujar el arbol avl*/
    int dibujarNodo(int x, int y, int valor);

    /*Metodo que permite dibujar las lineas de trazo*/
    void dibujarLinea(int x1, int y1, int x2, int y2);

    /*Pendiente cambiar*/
    void dibujarArbol(NodoFake* nodo, int x, int y, int offset) ;

    /*Funcion a eliminar SOLO ES SILUMACION SERA REMOVIDO CUANDO SE INTEGRE EL BACKEND REAL*/
    NodoFake* crearArbolPrueba();
};
#endif // PANTALLAARBOLAVL_H
