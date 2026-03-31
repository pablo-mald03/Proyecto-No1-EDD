#include "pantallabuscarrango.h"
#include "ui_pantallabuscarrango.h"
#include<QMessageBox>

PantallaBuscarRango::PantallaBuscarRango(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaBuscarRango)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaBuscarRango { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");
    this->ui->textArbolB->setReadOnly(true);
    this->ui->textListaOrdenada->setReadOnly(true);
    this->ui->textListaNoOrdenada->setReadOnly(true);

    this->ui->dateInferior->setCalendarPopup(true);
    this->ui->dateInferior->setDate(QDate::currentDate());

    this->ui->dateSuperior->setCalendarPopup(true);
    this->ui->dateSuperior->setDate(QDate::currentDate());
}
/*Destructor*/
PantallaBuscarRango::~PantallaBuscarRango()
{
    delete ui;
}



/*---***---Apartado de metodos que permiten comunicar a la UI los logs que se van a mostrar----***--*/
void PantallaBuscarRango::appendBLog(QString mensaje, QString color){

    QString htmlMensaje = mensaje.replace("\n", "<br>");

    this->ui->textArbolB->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}

void PantallaBuscarRango::appendListOrdenadaLog(QString mensaje, QString color){

    QString htmlMensaje = mensaje.replace("\n", "<br>");

    this->ui->textListaOrdenada->append(
        "<span style='color:" + color + ";'>" + htmlMensaje + "</span>"
        );
}

void PantallaBuscarRango::appendListNoOrdenadaLog(QString mensaje, QString color){

    QString htmlMensaje = mensaje.replace("\n", "<br>");

    this->ui->textListaNoOrdenada->append(
        "<span style='color:" + color + ";'>" + htmlMensaje + "</span>"
        );
}

/*Metodo que permite limpiar los componentes de insercion*/
void PantallaBuscarRango::limpiarDatos(){

    this->ui->dateSuperior->setDate(QDate::currentDate());
    this->ui->dateInferior->setDate(QDate::currentDate());
}

/*Metodo que permite limpiar los logs*/
void PantallaBuscarRango::limpiarLogs(){

    this->ui->textArbolB->clear();
    this->ui->textListaNoOrdenada->clear();
    this->ui->textListaOrdenada->clear();

    this->ui->labelTiempoNoOrdenada->setText("Tiempo de busqueda: 0 ms");
    this->ui->labelTiempoOrdenada->setText("Tiempo de busqueda: 0 ms");
    this->ui->labelTiempoB->setText("Tiempo de busqueda: 0 ms");
}

/*Metodo signal que permite limpiar los datos de una pantalla*/
void PantallaBuscarRango::limpiarPantalla(){
    this->limpiarDatos();
    this->limpiarLogs();
}


/*---***--- Fin del Apartado de metodos que permiten comunicar a la UI los logs que se van a mostrar----***--*/


/*Metodo para poder dar el tiempo total en procesar una instruccion
*
* 1 -> B
* 2 -> LISTA ORDENADA
* 3 -> LISTA NO ORDENADA
*
*/
void PantallaBuscarRango::mostrarTiempo(int estructura, double milisegundos){


    QString tiempoTexto = "Tiempo de busqueda: " + QString::number(milisegundos, 'f', 3) + " ms";

    switch(estructura){

    case 1:
        this->ui->labelTiempoB->setText(tiempoTexto);
        break;

    case 2:
        this->ui->labelTiempoOrdenada->setText(tiempoTexto);
        break;

    case 3:
        this->ui->labelTiempoNoOrdenada->setText(tiempoTexto);
        break;
    }

}


/*Metodo que permite limpiar datos*/
void PantallaBuscarRango::on_btnLimpiar_clicked()
{
    this->limpiarDatos();
    this->limpiarLogs();

}

/*Metodo que permite realizar la busqueda por rangos*/

void PantallaBuscarRango::on_btnBuscar_clicked()
{
    this->limpiarLogs();

    QDate fechaInf = ui->dateInferior->date();
    QDate fechaSup = ui->dateSuperior->date();

    if (fechaInf > fechaSup) {
        QMessageBox::warning(
            this,
            "Rango inválido",
            "La fecha inferior no puede ser mayor que la fecha superior"
            );
        return;
    }

    std::string fechaInferior = fechaInf.toString("yyyy-MM-dd").trimmed().toStdString();
    std::string fechaSuperior = fechaSup.toString("yyyy-MM-dd").trimmed().toStdString();

    emit buscarPorFechas(fechaInferior, fechaSuperior);

}

/*Metodo que permite viajar hacia la visualizacion de los arboles*/
void PantallaBuscarRango::on_btnVerArboles_clicked()
{
    emit verArboles();
}

