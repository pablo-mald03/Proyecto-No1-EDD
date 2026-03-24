#ifndef PANTALLABUSCARPORNOMBRE_H
#define PANTALLABUSCARPORNOMBRE_H

#include <QWidget>

namespace Ui {
class PantallaBuscarPorNombre;
}

class PantallaBuscarPorNombre : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaBuscarPorNombre(QWidget *parent = nullptr);
    ~PantallaBuscarPorNombre();

private:
    Ui::PantallaBuscarPorNombre *ui;
};

#endif // PANTALLABUSCARPORNOMBRE_H
