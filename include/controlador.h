#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <QObject>

class Controlador : public QObject
{
    Q_OBJECT
public:
    explicit Controlador(QObject *parent = nullptr);

public slots:
    void procesarCsv(const std::vector<QString> & data);

    void insercionProducto();


signals:

    /*Signals que permiten comunicarse con la pantalla del csv */
    void logArbolAvl(QString mensaje, QString color);
    void logArboB(QString mensaje, QString color);
    void logArbolBMas(QString mensaje, QString color);
    void logLista(QString mensaje, QString color);

    /*Signal que permite setear el tiempo en el label*/
    void tiempoProceso(int estructura, qint64 milisegundos);

    /*Fin de los Signals que permiten comunicarse con la pantalla del csv */


    /*Signals que permiten comunicarse con la pantalla de agregar */
    void logInsertArbolAvl(QString mensaje, QString color);
    void logInsertArbolB(QString mensaje, QString color);
    void logInsertArbolBMas(QString mensaje, QString color);
    void logInsertListaOrdenada(QString mensaje, QString color);
    void logInsertListaNoOrdenada(QString mensaje, QString color);

    /*Signal que permite setear el tiempo en el label*/
    void tiempoProcesoInsert (int estructura, qint64 milisegundos);

};

#endif // CONTROLADOR_H
