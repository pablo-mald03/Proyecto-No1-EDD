#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/*Region de importacion de la UI*/
#include"pantallaprincipal.h"
#include"controlador.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    PantallaPrincipal *inicio = nullptr;

    /*Controlador*/
    Controlador * controladorCrud;


    /*Metodos para navegar por la UI*/
    void mostrarInicio();
};
#endif // MAINWINDOW_H
