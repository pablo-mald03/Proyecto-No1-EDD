#ifndef PANTALLAVERARBOLES_H
#define PANTALLAVERARBOLES_H

#include <QWidget>

namespace Ui {
class PantallaVerArboles;
}

class PantallaVerArboles : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaVerArboles(QWidget *parent = nullptr);
    ~PantallaVerArboles();

private:
    Ui::PantallaVerArboles *ui;
};

#endif // PANTALLAVERARBOLES_H
