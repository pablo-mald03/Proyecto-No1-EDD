#include "pantallabuscarporcategoria.h"
#include "ui_pantallabuscarporcategoria.h"
#include<QMessageBox>

PantallaBuscarPorCategoria::PantallaBuscarPorCategoria(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaBuscarPorCategoria)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaBuscarPorCategoria { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");

    this->ui->textArbolBMas->setReadOnly(true);
    this->ui->textListaOrdenada->setReadOnly(true);
    this->ui->textListaNoOrdenada->setReadOnly(true);
}

/*Destructor*/
PantallaBuscarPorCategoria::~PantallaBuscarPorCategoria()
{
    delete ui;
}



/*---***---Apartado de metodos que permiten comunicar a la UI los logs que se van a mostrar----***--*/
void PantallaBuscarPorCategoria::appendBMasLog(QString mensaje, QString color){

    QString htmlMensaje = mensaje.replace("\n", "<br>");

    this->ui->textArbolBMas->append(
        "<span style='color:" + color + ";'>" + htmlMensaje + "</span>"
        );
}

void PantallaBuscarPorCategoria::appendListOrdenadaLog(QString mensaje, QString color){

    QString htmlMensaje = mensaje.replace("\n", "<br>");

    this->ui->textListaOrdenada->append(
        "<span style='color:" + color + ";'>" + htmlMensaje + "</span>"
        );
}

void PantallaBuscarPorCategoria::appendListNoOrdenadaLog(QString mensaje, QString color){

    QString htmlMensaje = mensaje.replace("\n", "<br>");

    this->ui->textListaNoOrdenada->append(
        "<span style='color:" + color + ";'>" + htmlMensaje + "</span>"
        );
}

/*Metodo que permite limpiar los componentes de insercion*/
void PantallaBuscarPorCategoria::limpiarDatos(){

    this->ui->textCategoria->clear();
}

/*Metodo que permite limpiar los logs*/
void PantallaBuscarPorCategoria::limpiarLogs(){

    this->ui->textArbolBMas->clear();
    this->ui->textListaNoOrdenada->clear();
    this->ui->textListaOrdenada->clear();

    this->ui->labelTiempoNoOrdenada->setText("Tiempo de busqueda:");
    this->ui->labelTiempoOrdenada->setText("Tiempo de busqueda:");
    this->ui->labelTiempoBMas->setText("Tiempo de busqueda:");

}


/*---***--- Fin del Apartado de metodos que permiten comunicar a la UI los logs que se van a mostrar----***--*/


/*Metodo para poder dar el tiempo total en procesar una instruccion
*
* 1 -> B+
* 2 -> LISTA ORDENADA
* 3 -> LISTA NO ORDENADA
*
*/
void PantallaBuscarPorCategoria::mostrarTiempo(int estructura, double milisegundos){


    QString tiempoTexto = "Tiempo de busqueda: " + QString::number(milisegundos, 'f', 3) + " ms";

    switch(estructura){

    case 1:
        this->ui->labelTiempoBMas->setText(tiempoTexto);
        break;

    case 2:
        this->ui->labelTiempoOrdenada->setText(tiempoTexto);
        break;

    case 3:
        this->ui->labelTiempoNoOrdenada->setText(tiempoTexto);
        break;
    }

}



/*Metodo que permite buscar por categoria*/
void PantallaBuscarPorCategoria::on_btnBuscar_clicked()
{

    this->limpiarLogs();

    QString nombreOriginal = ui->textCategoria->text();
    QString nombreLimpio = nombreOriginal.trimmed();

    if (nombreLimpio.isEmpty()) {

        QMessageBox::warning(
            this,
            "Campo requerido",
            "Ingrese un nombre valido (no puede esta vacio)."
            );

        return;
    }

    emit buscarPorCategoria(nombreLimpio.toStdString());
}

/*Metodo que permite limpiar los datos*/
void PantallaBuscarPorCategoria::on_btnLimpiar_clicked()
{
    this->limpiarDatos();
    this->limpiarLogs();
}

/*Metodo que permite viajar a ver arboles*/
void PantallaBuscarPorCategoria::on_btnVerArboles_clicked()
{
    emit verArboles();
}

/*Metodo signal que permite limpiar los datos de una pantalla*/
void PantallaBuscarPorCategoria::limpiarPantalla(){
    this->limpiarDatos();
    this->limpiarLogs();
}

