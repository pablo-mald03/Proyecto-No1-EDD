#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QGuiApplication>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Instrucciones indicadas para poder abrir en mas de una pantalla
    QList<QScreen *> screens = QGuiApplication::screens();

    if (screens.size() > 1) {
        QScreen *segundaPantalla = screens.at(1);
        QRect geometria = segundaPantalla->availableGeometry();

        this->setGeometry(
            QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                this->size(),
                geometria
                )
            );
    } else {
        this->move(QGuiApplication::primaryScreen()->availableGeometry().center() - this->rect().center());
    }

    this->inicio = new PantallaPrincipal(this);

    this->controladorCrud= new Controlador();
    this->ui->stackedWidget->addWidget(this->inicio);



    connect(this->inicio, &PantallaPrincipal::csvCargado, this->controladorCrud, &Controlador::procesarCsv);

    connect(this->controladorCrud, &Controlador::logArbolAvl,this->inicio, &PantallaPrincipal::appendAvlLog);

    connect(this->controladorCrud, &Controlador::logArboB,this->inicio, &PantallaPrincipal::appendBLog);

    connect(this->controladorCrud, &Controlador::logArbolBMas,this->inicio, &PantallaPrincipal::appendBMasLog);

    connect(this->controladorCrud, &Controlador::logLista,this->inicio, &PantallaPrincipal::appendListLog);

    connect(this->controladorCrud, &Controlador::tiempoProceso, this->inicio, &PantallaPrincipal::mostrarTiempo);

    mostrarInicio();

}

/*Metodo utilizado para poder inicializar el controlador*/
void MainWindow::mostrarInicio(){



    this->ui->stackedWidget->setCurrentWidget(this->inicio);
}


MainWindow::~MainWindow()
{
    delete ui;
}
