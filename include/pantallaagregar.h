#ifndef PANTALLAAGREGAR_H
#define PANTALLAAGREGAR_H

#include <QWidget>

namespace Ui {
class PantallaAgregar;
}

class PantallaAgregar : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaAgregar(QWidget *parent = nullptr);
    ~PantallaAgregar();

private:
    Ui::PantallaAgregar *ui;
};

#endif // PANTALLAAGREGAR_H
