#ifndef PANTALLAELIMINARPRODUCTO_H
#define PANTALLAELIMINARPRODUCTO_H

#include <QWidget>

namespace Ui {
class PantallaEliminarProducto;
}

class PantallaEliminarProducto : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaEliminarProducto(QWidget *parent = nullptr);
    ~PantallaEliminarProducto();

private:
    Ui::PantallaEliminarProducto *ui;
};

#endif // PANTALLAELIMINARPRODUCTO_H
