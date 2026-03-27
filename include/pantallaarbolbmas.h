#ifndef PANTALLAARBOLBMAS_H
#define PANTALLAARBOLBMAS_H

#include <QWidget>

namespace Ui {
class PantallaArbolBMas;
}

class PantallaArbolBMas : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaArbolBMas(QWidget *parent = nullptr);
    ~PantallaArbolBMas();

private:
    Ui::PantallaArbolBMas *ui;
};

#endif // PANTALLAARBOLBMAS_H
