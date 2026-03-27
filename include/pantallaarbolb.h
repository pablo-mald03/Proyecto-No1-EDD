#ifndef PANTALLAARBOLB_H
#define PANTALLAARBOLB_H

#include <QWidget>

namespace Ui {
class PantallaArbolB;
}

class PantallaArbolB : public QWidget
{
    Q_OBJECT

public:
    explicit PantallaArbolB(QWidget *parent = nullptr);
    ~PantallaArbolB();

private:
    Ui::PantallaArbolB *ui;
};

#endif // PANTALLAARBOLB_H
