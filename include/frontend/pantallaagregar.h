#ifndef PANTALLAAGREGAR_H
#define PANTALLAAGREGAR_H

#include <QWidget>

namespace Ui {
class PantallaAgregar;
}

class PantallaAgregar : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaAgregar(QWidget *parent = nullptr);
    ~PantallaAgregar();

private:
    Ui::PantallaAgregar *ui;

    void limpiarDatos();

    void limpiarLogs();

signals:
    /*Pendiente insertar*/
    void insertarProducto();
    /*Metodo que permite viajar a mostrar los arboles*/
    void verArboles();

public slots:
    /*Metodos que permiten comunicar las lineas leidas por los arboles o listas enlazadas*/
    void appendAvlLog(QString mensaje, QString color);
    void appendBLog(QString mensaje, QString color);
    void appendBMasLog(QString mensaje, QString color);
    void appendListOrdenadaLog(QString mensaje, QString color);
    void appendListNoOrdenadaLog(QString mensaje, QString color);

    void mostrarTiempo(int estructura, qint64 milisegundos);

    void limpiarPantalla();


private slots:
    void on_btnAgregar_clicked();
    void on_btnVerArboles_clicked();
    void on_btnLimpiar_clicked();
};

#endif // PANTALLAAGREGAR_H
