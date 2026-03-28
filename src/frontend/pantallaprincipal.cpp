#include "pantallaprincipal.h"
#include "ui/ui_pantallaprincipal.h"
#include<QFileDialog>
#include<QFile>
#include<QTextStream>

PantallaPrincipal::PantallaPrincipal(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PantallaPrincipal)
{
    ui->setupUi(this);


    this->setAttribute(Qt::WA_StyledBackground, true);

    //Se aplica el estilo al contenedor padre
    this->setStyleSheet("QWidget#PantallaPrincipal { "
                        "border-image: url(:/assets/fondo/fondooverlay.png) 0 0 0 0 stretch stretch; "
                        "} ");

    this->ui->textEditCsv->setReadOnly(true);
    this->ui->textEditAvl->setReadOnly(true);
    this->ui->textEditB->setReadOnly(true);
    this->ui->textEditBMas->setReadOnly(true);
    this->ui->textEditLista->setReadOnly(true);
}

/*Destructor*/
PantallaPrincipal::~PantallaPrincipal()
{
    delete ui;
}


/*Metodo que permite seleccionar el archivo csv*/
void PantallaPrincipal::on_btnCargar_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Seleccionar CSV",
        "",
        "CSV Files (*.csv);;All Files (*)"
        );

    if(fileName.isEmpty()){
        return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        this->ui->textEditCsv->append("<span style='color:red;'>Error al abrir archivo</span>");
        return;
    }

    QTextStream in(&file);

    std::vector<QString> buffer;

    this->ui->textEditCsv->clear();


    while (!in.atEnd()) {
        QString line = in.readLine();
        buffer.push_back(line);
    }

    file.close();

    emit csvCargado(buffer);

}


/*---***---Apartado de metodos que permiten comunicar a la UI los logs que se van a mostrar----***--*/

void PantallaPrincipal::appendLogCarga(QString mensaje, QString color){

    this->ui->textEditCsv->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}

void PantallaPrincipal::appendAvlLog(QString mensaje, QString color){

    this->ui->textEditAvl->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}


void PantallaPrincipal::appendBLog(QString mensaje, QString color){

    this->ui->textEditB->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}

void PantallaPrincipal::appendBMasLog(QString mensaje, QString color){

    this->ui->textEditBMas->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}

void PantallaPrincipal::appendListLog(QString mensaje, QString color){

    this->ui->textEditLista->append(
        "<span style='color:" + color + ";'>" + mensaje + "</span>"
        );
}


/*---***--- Fin del Apartado de metodos que permiten comunicar a la UI los logs que se van a mostrar----***--*/


/*Metodo para poder dar el tiempo total en procesar una instruccion
*
* 1 -> AVL
* 2 -> B
* 3 -> B+
* 4 -> LISTA
*
*/
void PantallaPrincipal::mostrarTiempo(int estructura, qint64 milisegundos){

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
            this->ui->labelTiempoLista->setText("Tiempo total: " + QString::number(milisegundos) + " ms");
            break;
    }

}

//Metodo que permite descargar el log de errores
void PantallaPrincipal::on_btnErrores_clicked()
{


}

