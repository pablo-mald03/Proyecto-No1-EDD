#ifndef PANTALLABUSCARPORCATEGORIA_H
#define PANTALLABUSCARPORCATEGORIA_H

#include <QWidget>

namespace Ui {
class PantallaBuscarPorCategoria;
}

class PantallaBuscarPorCategoria : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaBuscarPorCategoria(QWidget *parent = nullptr);
    ~PantallaBuscarPorCategoria();

signals:
    /*Pendiente buscar*/
    void buscarPorCategoria(std::string nombre);
    /*Metodo que permite viajar a mostrar los arboles*/
    void verArboles();

public slots:
    void appendBMasLog(QString mensaje, QString color);
    void appendListOrdenadaLog(QString mensaje, QString color);
    void appendListNoOrdenadaLog(QString mensaje, QString color);

    void mostrarTiempo(int estructura, qint64 milisegundos);

    void limpiarPantalla();


private slots:
    void on_btnBuscar_clicked();

    void on_btnLimpiar_clicked();

    void on_btnVerArboles_clicked();

private:
    Ui::PantallaBuscarPorCategoria *ui;

    void limpiarDatos();
    void limpiarLogs();
};

#endif // PANTALLABUSCARPORCATEGORIA_H
