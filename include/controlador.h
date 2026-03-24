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

/*Signals que permiten comunicarse con el csv */
signals:
    void logArbolAvl(QString mensaje, QString color);
    void logArboB(QString mensaje, QString color);
    void logArbolBMas(QString mensaje, QString color);
    void logLista(QString mensaje, QString color);

    /*Signal que permite setear el tiempo en el label*/
    void tiempoProceso(int estructura, qint64 milisegundos);

};

#endif // CONTROLADOR_H
