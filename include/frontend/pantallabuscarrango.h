#ifndef PANTALLABUSCARRANGO_H
#define PANTALLABUSCARRANGO_H

#include <QWidget>

namespace Ui {
class PantallaBuscarRango;
}

class PantallaBuscarRango : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaBuscarRango(QWidget *parent = nullptr);
    ~PantallaBuscarRango();

private:
    Ui::PantallaBuscarRango *ui;
    void limpiarDatos();
    void limpiarLogs();

signals:

    void buscarPorFechas(std::string limiteInferior, std::string limiteSuperior);

    /*Metodo que permite viajar a mostrar los arboles*/
    void verArboles();

public slots:
    void appendBLog(QString mensaje, QString color);
    void appendListOrdenadaLog(QString mensaje, QString color);
    void appendListNoOrdenadaLog(QString mensaje, QString color);

    void mostrarTiempo(int estructura, qint64 milisegundos);

    void limpiarPantalla();

private slots:
    void on_btnLimpiar_clicked();
    void on_btnBuscar_clicked();
    void on_btnVerArboles_clicked();
};

#endif // PANTALLABUSCARRANGO_H
