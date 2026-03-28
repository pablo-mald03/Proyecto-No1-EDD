#ifndef PANTALLABUSCARPORNOMBRE_H
#define PANTALLABUSCARPORNOMBRE_H

#include <QWidget>

namespace Ui {
class PantallaBuscarPorNombre;
}

class PantallaBuscarPorNombre : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaBuscarPorNombre(QWidget *parent = nullptr);
    ~PantallaBuscarPorNombre();

signals:
    /*Pendiente buscar*/
    void buscarPorNombre(std::string nombre);
    /*Metodo que permite viajar a mostrar los arboles*/
    void verArboles();

public slots:
    void appendAvlLog(QString mensaje, QString color);
    void appendListOrdenadaLog(QString mensaje, QString color);
    void appendListNoOrdenadaLog(QString mensaje, QString color);

    void mostrarTiempo(int estructura, qint64 milisegundos);

    void limpiarPantalla();

private slots:
    void on_btnBuscar_clicked();

    void on_btnLimpiar_clicked();

    void on_btnVerArboles_clicked();

private:
    Ui::PantallaBuscarPorNombre *ui;

    void limpiarDatos();
    void limpiarLogs();
};

#endif // PANTALLABUSCARPORNOMBRE_H
