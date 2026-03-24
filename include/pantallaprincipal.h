#ifndef PANTALLAPRINCIPAL_H
#define PANTALLAPRINCIPAL_H

#include <QWidget>

namespace Ui {
class PantallaPrincipal;
}

class PantallaPrincipal : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaPrincipal(QWidget *parent = nullptr);
    ~PantallaPrincipal();

private slots:
    void on_btnCargar_clicked();

signals:
    void csvCargado(const std::vector<QString>& data);


public slots:

    /*Metodos que permiten comunicar las lineas leidas por los arboles o lista enlazada*/
    void appendAvlLog(QString mensaje, QString color);
    void appendBLog(QString mensaje, QString color);
    void appendBMasLog(QString mensaje, QString color);
    void appendListLog(QString mensaje, QString color);

    void mostrarTiempo(int estructura, qint64 milisegundos);

private:
    Ui::PantallaPrincipal *ui;
};

#endif // PANTALLAPRINCIPAL_H
