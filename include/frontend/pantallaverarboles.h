#ifndef PANTALLAVERARBOLES_H
#define PANTALLAVERARBOLES_H

#include <QWidget>
#include"pantallaarbolavl.h"
#include"pantallaarbolb.h"
#include"pantallaarbolbmas.h"

namespace Ui {
class PantallaVerArboles;
}

class PantallaVerArboles : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaVerArboles(QWidget *parent = nullptr);
    ~PantallaVerArboles();

    /*Metodo que permite generar el reinicio y foco a la primera vista de los arboles*/
    void reestablecerVistas();

signals:
    void solicitarArbolAvl();
    void solicitarArbolB();
    void solicitarArbolBMas();

public slots:
    /*pendiente redefnir tipos*/
    void recibirArbolAvl(int * arbol);
    void recibirArbolB(int * arbol);
    void recibirArbolBMas(int * arbol);

private slots:
    void on_btnSiguiente_clicked();

    void on_btnAnterior_clicked();


    /*Metodos propios para solicitar estructuras de arboles*/
    void onSolicitarArbolAvl();
    void onSolicitarArbolB();
    void onSolicitarArbolBMas();

private:
    Ui::PantallaVerArboles *ui;

    int contadorPantallas;

    /*Instancias de pantallas*/
    PantallaArbolAvl *pantallaAvl = nullptr;

    PantallaArbolB *pantallaB = nullptr;

    PantallaArbolBMas * pantallaBMas = nullptr;


    /*Metodos de carga entre pantallas*/
    void mostrarVistaAvl();
    void mostrarVistaB();
    void mostrarVistaBMas();
    void refrescarVista();

};

#endif // PANTALLAVERARBOLES_H
