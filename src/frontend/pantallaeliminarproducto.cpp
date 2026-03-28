#include "pantallaeliminarproducto.h"
#include "ui_pantallaeliminarproducto.h"
#include<QMessageBox>

PantallaEliminarProducto::PantallaEliminarProducto(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaEliminarProducto)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaEliminarProducto { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");

    this->ui->textArbolAvl->setReadOnly(true);
    this->ui->textArbolB->setReadOnly(true);
    this->ui->textArbolBMas->setReadOnly(true);
    this->ui->textEditListNoOrdenada->setReadOnly(true);
    this->ui->textEditListOrdenada->setReadOnly(true);
}


/*Destructor*/
PantallaEliminarProducto::~PantallaEliminarProducto()
{
    delete ui;
}


/*---***---Apartado de metodos que permiten comunicar a la UI los logs que se van a mostrar----***--*/
void PantallaEliminarProducto::appendAvlLog(QString mensaje, QString color){

    this->ui->textArbolAvl->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}


void PantallaEliminarProducto::appendBLog(QString mensaje, QString color){

    this->ui->textArbolB->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}

void PantallaEliminarProducto::appendBMasLog(QString mensaje, QString color){

    this->ui->textArbolBMas->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}

void PantallaEliminarProducto::appendListOrdenadaLog(QString mensaje, QString color){

    this->ui->textEditListOrdenada->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}

void PantallaEliminarProducto::appendListNoOrdenadaLog(QString mensaje, QString color){

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
void PantallaEliminarProducto::mostrarTiempo(int estructura, qint64 milisegundos){

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

/*Metodo signal que permite limpiar los datos de una pantalla*/
void PantallaEliminarProducto::limpiarPantalla(){
    this->limpiarDatos();
    this->limpiarLogs();
}


/*Metodo que permite limpiar los componentes de insercion*/
void PantallaEliminarProducto::limpiarDatos(){

    this->ui->textBarra->clear();

}

/*Metodo que permite limpiar los logs*/
void PantallaEliminarProducto::limpiarLogs(){

    this->ui->textArbolB->clear();
    this->ui->textArbolAvl->clear();
    this->ui->textArbolBMas->clear();
    this->ui->textEditListNoOrdenada->clear();
    this->ui->textEditListOrdenada->clear();

    this->ui->labelTiempoListNoOrdenada->setText("Tiempo de eliminacion:");
    this->ui->labelTiempoListOrdenada->setText("Tiempo de eliminacion:");
    this->ui->labelTiempoAvl->setText("Tiempo de eliminacion:");
    this->ui->labelTiempoB->setText("Tiempo de eliminacion:");
    this->ui->labelTiempoBMas->setText("Tiempo de eliminacion:");
}



/*Boton que permite eliminar un producto*/
void PantallaEliminarProducto::on_btnEliminar_clicked()
{
    QString codigo = ui->textBarra->text().trimmed();

    if (codigo.isEmpty()) {
        QMessageBox::warning(
            this,
            "Campo requerido",
            "Ingrese un codigo de barras válido"
            );
        return;
    }


    QMessageBox::StandardButton respuesta;

    respuesta = QMessageBox::question(
        this,
        "Confirmar eliminación",
        "¿Está seguro que desea eliminar el producto con codigo:\n" + codigo + "?",
        QMessageBox::Yes | QMessageBox::No
        );

    if (respuesta == QMessageBox::No) {
        return;
    }

    emit eliminarProducto(codigo.toStdString());
}


void PantallaEliminarProducto::on_btnLimpiar_clicked()
{
    this->limpiarDatos();
    this->limpiarLogs();
}


void PantallaEliminarProducto::on_btnVerArboles_clicked()
{
    emit verArboles();
}

