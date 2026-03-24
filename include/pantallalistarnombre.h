#ifndef PANTALLALISTARNOMBRE_H
#define PANTALLALISTARNOMBRE_H

#include <QWidget>

namespace Ui {
class PantallaListarNombre;
}

class PantallaListarNombre : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaListarNombre(QWidget *parent = nullptr);
    ~PantallaListarNombre();

private:
    Ui::PantallaListarNombre *ui;
};

#endif // PANTALLALISTARNOMBRE_H
