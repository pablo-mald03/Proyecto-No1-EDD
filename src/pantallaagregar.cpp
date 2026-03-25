#include "pantallaagregar.h"
#include "ui_pantallaagregar.h"

PantallaAgregar::PantallaAgregar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaAgregar)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaAgregar { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");

    this->ui->dateExpiracion->setCalendarPopup(true);

}

/*Destructor*/
PantallaAgregar::~PantallaAgregar()
{
    delete ui;
}


/*---***---Apartado de metodos que permiten comunicar a la UI los logs que se van a mostrar----***--*/
void PantallaAgregar::appendAvlLog(QString mensaje, QString color){

    this->ui->textArbolAvl->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}


void PantallaAgregar::appendBLog(QString mensaje, QString color){

    this->ui->textArbolB->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}

void PantallaAgregar::appendBMasLog(QString mensaje, QString color){

    this->ui->textArbolBMas->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}

void PantallaAgregar::appendListOrdenadaLog(QString mensaje, QString color){

    this->ui->textEditListOrdenada->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}

void PantallaAgregar::appendListNoOrdenadaLog(QString mensaje, QString color){

    this->ui->textEditListNoOrdenada->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}


/*---***--- Fin del Apartado de metodos que permiten comunicar a la UI los logs que se van a mostrar----***--*/


/*Metodo para poder dar el tiempo total en procesar una instruccion
*
* 1 -> AVL
* 2 -> B
* 3 -> B+
* 4 -> LISTA ORDENADA
* 5 -> LISTA NO ORDENADA
*
*/
void PantallaAgregar::mostrarTiempo(int estructura, qint64 milisegundos){

    switch(estructura){

    case 1:
        this->ui->labelTiempoAvl->setText("Tiempo total: " + QString::number(milisegundos) + " ms");
        break;

    case 2:
        this->ui->labelTiempoB->setText("Tiempo total: " + QString::number(milisegundos) + " ms");
        break;

    case 3:
        this->ui->labelTiempoBMas->setText("Tiempo total: " + QString::number(milisegundos) + " ms");
        break;

    case 4:
        this->ui->labelTiempoListOrdenada->setText("Tiempo total: " + QString::number(milisegundos) + " ms");
        break;
    case 5:
        this->ui->labelTiempoListNoOrdenada->setText("Tiempo total: " + QString::number(milisegundos) + " ms");
        break;
    }

}



/*Metodo que permite insertar un producto nuevo en todas las estructuras*/
void PantallaAgregar::on_btnAgregar_clicked()
{
    std::string fecha = ui->dateExpiracion->date().toString("yyyy-MM-dd").toStdString();
    /*Pendiente*/
    emit insertarProducto();
}

/*Metodo que permite ir a ver el estado de los arboles*/
void PantallaAgregar::on_btnVerArboles_clicked()
{
    emit verArboles();
}


/*Metodo que permite limpiar los componentes de insercion*/

