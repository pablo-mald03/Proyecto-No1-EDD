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

    QString htmlMensaje = mensaje.replace("\n", "<br>");

    this->ui->textArbolAvl->append(
        "<span style='color:" + color + ";'>" + htmlMensaje + "</span>"
        );
}

void PantallaBuscarPorNombre::appendListOrdenadaLog(QString mensaje, QString color){

    QString htmlMensaje = mensaje.replace("\n", "<br>");

    this->ui->textListaOrdenada->append(
        "<span style='color:" + color + ";'>" + htmlMensaje + "</span>"
        );
}

void PantallaBuscarPorNombre::appendListNoOrdenadaLog(QString mensaje, QString color){

    QString htmlMensaje = mensaje.replace("\n", "<br>");

    this->ui->textListaNoOrdenada->append(
        "<span style='color:" + color + ";'>" + htmlMensaje + "</span>"
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
void PantallaBuscarPorNombre::mostrarTiempo(int estructura, double milisegundos){

    QString tiempoTexto = "Tiempo de busqueda: " + QString::number(milisegundos, 'f', 3) + " ms";

    switch(estructura){

    case 1:
        this->ui->labelTiempoAvl->setText(tiempoTexto);
        break;

    case 2:
        this->ui->labelTiempoOrdenada->setText(tiempoTexto);
        break;

    case 3:
        this->ui->labelTiempoNoOrdenada->setText(tiempoTexto);
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

    this->ui->labelTiempoNoOrdenada->setText("Tiempo de busqueda: 0 ms");
    this->ui->labelTiempoOrdenada->setText("Tiempo de busqueda: 0 ms");
    this->ui->labelTiempoAvl->setText("Tiempo de busqueda: 0 ms");

}

/*Metodo que permite emitir la signal para poder buscar*/
void PantallaBuscarPorNombre::on_btnBuscar_clicked()
{

    this->limpiarLogs();

    QString nombreOriginal = ui->textNombre->text();
    QString nombreLimpio = nombreOriginal.trimmed();

    if (nombreLimpio.isEmpty()) {

        QMessageBox::warning(
            this,
            "Campo requerido",
            "Ingrese un nombre valido (no puede esta vacio)."
            );

        return;
    }

    emit buscarPorNombre(nombreLimpio.toStdString());
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
