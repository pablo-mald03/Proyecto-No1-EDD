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

/*Region de metodos para poder mostrar las diferentes vistas*/
void MainWindow::mostrarInicio(){

    this->ui->labelTasks->setText("Menu Principal");
    this->ui->stackedWidget->setCurrentWidget(this->inicio);
}


/*Metodo utilizado para poder navegar a la ventana de agregar*/
void MainWindow::mostrarAgregar(){

    if (!this->pantallaAgregar) {

        this->pantallaAgregar = new PantallaAgregar(this);

        ui->stackedWidget->addWidget(this->pantallaAgregar);

        //Se conectan las signals para poder interactuar con la pantalla
        connect(this->controladorCrud, &Controlador::logInsertArbolAvl,this->pantallaAgregar, &PantallaAgregar::appendAvlLog);

        connect(this->controladorCrud, &Controlador::logInsertArbolB,this->pantallaAgregar, &PantallaAgregar::appendBLog);

        connect(this->controladorCrud, &Controlador::logInsertArbolBMas,this->pantallaAgregar, &PantallaAgregar::appendBMasLog);

        connect(this->controladorCrud, &Controlador::logInsertListaOrdenada,this->pantallaAgregar, &PantallaAgregar::appendListOrdenadaLog);

        connect(this->controladorCrud, &Controlador::logInsertListaNoOrdenada,this->pantallaAgregar, &PantallaAgregar::appendListNoOrdenadaLog);

        connect(this->controladorCrud, &Controlador::tiempoProcesoInsert, this->pantallaAgregar, &PantallaAgregar::mostrarTiempo);

        connect(this->pantallaAgregar, &PantallaAgregar::insertarProducto, this->controladorCrud, &Controlador::insercionProducto);

        connect(this->pantallaAgregar, &PantallaAgregar::verArboles, this, &MainWindow::mostrarVerArboles);
    }

    this->ui->labelTasks->setText("Agregar Productos");
    this->ui->stackedWidget->setCurrentWidget(this->pantallaAgregar);
}

/*Metodo utilizado para poder navegar a la ventana de buscar por nombre*/
void MainWindow::mostrarBuscarNombre(){

    if (!this->pantallaBusquedaNombre) {

        this->pantallaBusquedaNombre = new PantallaBuscarPorNombre(this);

        ui->stackedWidget->addWidget(this->pantallaBusquedaNombre);

        //Se conectan las signlas para poder interactuar con las pantallas PENDIENTE
        /* connect(this->pantallaSelect, &PantallaSeleccion::solicitarModalidad, this, &MainWindow::mostrarModalidad);

        connect(this->pantallaSelect, &PantallaSeleccion::solicitarRegresoInicio, this, [this](){
            this->estaConfigurando = false;
            ui->gestorVentanas->setCurrentWidget(this->inicio);
        });*/
    }

    this->ui->labelTasks->setText("Busqueda por nombre");
    this->ui->stackedWidget->setCurrentWidget(this->pantallaBusquedaNombre);

}

/*Metodo utilizado para poder navegar a la ventana de buscar por categoria*/
void MainWindow::mostrarBuscarCategoria(){

    if (!this->pantallaBusquedaCategoria) {

        this->pantallaBusquedaCategoria = new PantallaBuscarPorCategoria(this);

        ui->stackedWidget->addWidget(this->pantallaBusquedaCategoria);

        //Se conectan las signlas para poder interactuar con las pantallas PENDIENTE
        /* connect(this->pantallaSelect, &PantallaSeleccion::solicitarModalidad, this, &MainWindow::mostrarModalidad);

        connect(this->pantallaSelect, &PantallaSeleccion::solicitarRegresoInicio, this, [this](){
            this->estaConfigurando = false;
            ui->gestorVentanas->setCurrentWidget(this->inicio);
        });*/
    }

    this->ui->labelTasks->setText("Busqueda por categoria");
    this->ui->stackedWidget->setCurrentWidget(this->pantallaBusquedaCategoria);

}

/*Metodo utilizado para poder navegar a la ventana de buscar por rango*/
void MainWindow::mostrarBuscarRango(){

    if (!this->pantallaBusquedaRango) {

        this->pantallaBusquedaRango = new PantallaBuscarRango(this);

        ui->stackedWidget->addWidget(this->pantallaBusquedaRango);

        //Se conectan las signlas para poder interactuar con las pantallas PENDIENTE
        /* connect(this->pantallaSelect, &PantallaSeleccion::solicitarModalidad, this, &MainWindow::mostrarModalidad);

        connect(this->pantallaSelect, &PantallaSeleccion::solicitarRegresoInicio, this, [this](){
            this->estaConfigurando = false;
            ui->gestorVentanas->setCurrentWidget(this->inicio);
        });*/
    }

    this->ui->labelTasks->setText("Busqueda por rango de caducidad");
    this->ui->stackedWidget->setCurrentWidget(this->pantallaBusquedaRango);

}

/*Metodo utilizado para poder navegar a la ventana de buscar por rango*/
void MainWindow::mostrarEliminar(){

    if (!this->pantallaEliminar) {

        this->pantallaEliminar = new PantallaEliminarProducto(this);

        ui->stackedWidget->addWidget(this->pantallaEliminar);

        //Se conectan las signlas para poder interactuar con las pantallas PENDIENTE
        /* connect(this->pantallaSelect, &PantallaSeleccion::solicitarModalidad, this, &MainWindow::mostrarModalidad);

        connect(this->pantallaSelect, &PantallaSeleccion::solicitarRegresoInicio, this, [this](){
            this->estaConfigurando = false;
            ui->gestorVentanas->setCurrentWidget(this->inicio);
        });*/
    }

    this->ui->labelTasks->setText("Eliminar productos");
    this->ui->stackedWidget->setCurrentWidget(this->pantallaEliminar);

}

/*Metodo utilizado para poder navegar a la ventana de listar por nombres*/
void MainWindow::mostrarListarNombre(){

    if (!this->pantallaListarNombre) {

        this->pantallaListarNombre = new PantallaListarNombre(this);

        ui->stackedWidget->addWidget(this->pantallaListarNombre);

        //Se conectan las signlas para poder interactuar con las pantallas PENDIENTE
        /* connect(this->pantallaSelect, &PantallaSeleccion::solicitarModalidad, this, &MainWindow::mostrarModalidad);

        connect(this->pantallaSelect, &PantallaSeleccion::solicitarRegresoInicio, this, [this](){
            this->estaConfigurando = false;
            ui->gestorVentanas->setCurrentWidget(this->inicio);
        });*/
    }

    this->ui->labelTasks->setText("Listar por nombre");
    this->ui->stackedWidget->setCurrentWidget(this->pantallaListarNombre);

}

/*Metodo utilizado para poder navegar a la ventana de comparar busquedas*/
void MainWindow::mostrarCompararBusqueda(){

    if (!this->pantallaComparar) {

        this->pantallaComparar = new PantallaCompararBusqueda(this);

        ui->stackedWidget->addWidget(this->pantallaComparar);

        //Se conectan las signlas para poder interactuar con las pantallas PENDIENTE
        /* connect(this->pantallaSelect, &PantallaSeleccion::solicitarModalidad, this, &MainWindow::mostrarModalidad);

        connect(this->pantallaSelect, &PantallaSeleccion::solicitarRegresoInicio, this, [this](){
            this->estaConfigurando = false;
            ui->gestorVentanas->setCurrentWidget(this->inicio);
        });*/
    }

    this->ui->labelTasks->setText("Comparar busquedas");
    this->ui->stackedWidget->setCurrentWidget(this->pantallaComparar);

}

/*Metodo utilizado para poder navegar a la ventana de ver arboles*/
void MainWindow::mostrarVerArboles(){

    if (!this->pantallaVerArboles) {

        this->pantallaVerArboles = new PantallaVerArboles(this);

        ui->stackedWidget->addWidget(this->pantallaVerArboles);

        //Se conectan las signlas para poder interactuar con las pantallas PENDIENTE
        /* connect(this->pantallaSelect, &PantallaSeleccion::solicitarModalidad, this, &MainWindow::mostrarModalidad);

        connect(this->pantallaSelect, &PantallaSeleccion::solicitarRegresoInicio, this, [this](){
            this->estaConfigurando = false;
            ui->gestorVentanas->setCurrentWidget(this->inicio);
        });*/
    }

    this->ui->labelTasks->setText("Visualizar Arboles");
    this->ui->stackedWidget->setCurrentWidget(this->pantallaVerArboles);

}


MainWindow::~MainWindow()
{
    delete ui;
}

/*Metodo que permite navegar al inicio*/
void MainWindow::on_btnCsv_clicked()
{
    mostrarInicio();
}

/*Metodo que permite navegar a la pantalla de agregar*/
void MainWindow::on_btnAgregar_clicked()
{
    this->mostrarAgregar();
}

/*Metodo que permite navegar a la pantalla de buscar por nombre*/
void MainWindow::on_btnBuscarNombre_clicked()
{
    this->mostrarBuscarNombre();
}

/*Metodo que permite navegar a la pantalla de buscar por categoria*/
void MainWindow::on_btnBuscarCategoria_clicked()
{
    this->mostrarBuscarCategoria();
}

/*Metodo que permite navegar a la pantalla de buscar por rango de caducidad*/
void MainWindow::on_btnBuscarCaducidad_clicked()
{
    this->mostrarBuscarRango();
}

/*Metodo que permite navegar a la pantalla de eliminar un producto*/
void MainWindow::on_btnEliminar_clicked()
{
    this->mostrarEliminar();
}

/*Metodo que permite navegar a la pantalla de listar por nombre*/
void MainWindow::on_btnListarNombre_clicked()
{
    this->mostrarListarNombre();
}

/*Metodo que permite navegar a la pantalla de comparar busquedas*/
void MainWindow::on_btnCompararBusqueda_clicked()
{
    this->mostrarCompararBusqueda();
}

/*Metodo que permite navegar a la pantalla de mostrar el estado de los arboles*/
void MainWindow::on_btnArboles_clicked()
{
    this->mostrarVerArboles();
}

