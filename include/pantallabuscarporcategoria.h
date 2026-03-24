#ifndef PANTALLABUSCARPORCATEGORIA_H
#define PANTALLABUSCARPORCATEGORIA_H

#include <QWidget>

namespace Ui {
class PantallaBuscarPorCategoria;
}

class PantallaBuscarPorCategoria : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaBuscarPorCategoria(QWidget *parent = nullptr);
    ~PantallaBuscarPorCategoria();

private:
    Ui::PantallaBuscarPorCategoria *ui;
};

#endif // PANTALLABUSCARPORCATEGORIA_H
