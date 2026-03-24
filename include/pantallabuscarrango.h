#ifndef PANTALLABUSCARRANGO_H
#define PANTALLABUSCARRANGO_H

#include <QWidget>

namespace Ui {
class PantallaBuscarRango;
}

class PantallaBuscarRango : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaBuscarRango(QWidget *parent = nullptr);
    ~PantallaBuscarRango();

private:
    Ui::PantallaBuscarRango *ui;
};

#endif // PANTALLABUSCARRANGO_H
