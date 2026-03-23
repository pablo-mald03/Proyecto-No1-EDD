#ifndef PANTALLAPRINCIPAL_H
#define PANTALLAPRINCIPAL_H

#include <QWidget>

namespace Ui {
class PantallaPrincipal;
}

class PantallaPrincipal : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaPrincipal(QWidget *parent = nullptr);
    ~PantallaPrincipal();

private:
    Ui::PantallaPrincipal *ui;
};

#endif // PANTALLAPRINCIPAL_H
