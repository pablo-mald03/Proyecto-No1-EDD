#ifndef PANTALLAARBOLBMAS_H
#define PANTALLAARBOLBMAS_H

#include <QWidget>
#include<QGraphicsView>
#include <QWheelEvent>
#include <QGraphicsTextItem>

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
};

#endif // PANTALLAARBOLBMAS_H
