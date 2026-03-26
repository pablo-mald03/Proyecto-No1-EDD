#include "pantallabuscarpornombre.h"
#include "ui_pantallabuscarpornombre.h"
#include<QMessageBox>

PantallaBuscarPorNombre::PantallaBuscarPorNombre(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaBuscarPorNombre)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaBuscarPorNombre { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");

    this->ui->textArbolAvl->setReadOnly(true);
    this->ui->textListaOrdenada->setReadOnly(true);
    this->ui->textListaNoOrdenada->setReadOnly(true);
}
/*Destructor*/
PantallaBuscarPorNombre::~PantallaBuscarPorNombre()
{
    delete ui;
}



/*---***---Apartado de metodos que permiten comunicar a la UI los logs que se van a mostrar----***--*/
void PantallaBuscarPorNombre::appendAvlLog(QString mensaje, QString color){

    this->ui->textArbolAvl->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}

void PantallaBuscarPorNombre::appendListOrdenadaLog(QString mensaje, QString color){

    this->ui->textListaOrdenada->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}

void PantallaBuscarPorNombre::appendListNoOrdenadaLog(QString mensaje, QString color){

    this->ui->textListaNoOrdenada->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}


/*---***--- Fin del Apartado de metodos que permiten comunicar a la UI los logs que se van a mostrar----***--*/


/*Metodo para poder dar el tiempo total en procesar una instruccion
*
* 1 -> AVL
* 2 -> LISTA ORDENADA
* 3 -> LISTA NO ORDENADA
*
*/
void PantallaBuscarPorNombre::mostrarTiempo(int estructura, qint64 milisegundos){

    switch(estructura){

    case 1:
        this->ui->labelTiempoAvl->setText("Tiempo de busqueda: " + QString::number(milisegundos) + " ms");
        break;

    case 2:
        this->ui->labelTiempoOrdenada->setText("Tiempo de busqueda: " + QString::number(milisegundos) + " ms");
        break;

    case 3:
        this->ui->labelTiempoNoOrdenada->setText("Tiempo de busqueda: " + QString::number(milisegundos) + " ms");
        break;
    }

}

/*Metodo que permite limpiar los componentes de insercion*/
void PantallaBuscarPorNombre::limpiarDatos(){

    this->ui->textNombre->clear();
}

/*Metodo que permite limpiar los logs*/
void PantallaBuscarPorNombre::limpiarLogs(){

    this->ui->textArbolAvl->clear();
    this->ui->textListaNoOrdenada->clear();
    this->ui->textListaOrdenada->clear();

    this->ui->labelTiempoNoOrdenada->setText("Tiempo de busqueda:");
    this->ui->labelTiempoOrdenada->setText("Tiempo de busqueda:");
    this->ui->labelTiempoAvl->setText("Tiempo de busqueda:");

}

/*Metodo que permite emitir la signal para poder buscar*/
void PantallaBuscarPorNombre::on_btnBuscar_clicked()
{

    this->limpiarLogs();

    QString nombre = ui->textNombre->text();

    if (nombre.trimmed().isEmpty()) {

        QMessageBox::warning(
            this,
            "Campo requerido",
            "Ingrese un nombre valido"
            );

        return;
    }

    emit buscarPorNombre(nombre.toStdString());

}

/*Metodo que permite emitir la signal para poder limpiar*/
void PantallaBuscarPorNombre::on_btnLimpiar_clicked()
{
    this->limpiarDatos();
    this->limpiarLogs();
}

/*Metodo que permite redireccionar para poder consultar los arboles*/
void PantallaBuscarPorNombre::on_btnVerArboles_clicked()
{
    emit verArboles();
}


/*Metodo signal que permite limpiar los datos de una pantalla*/
void PantallaBuscarPorNombre::limpiarPantalla(){
    this->limpiarDatos();
    this->limpiarLogs();
}
