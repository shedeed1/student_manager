#ifndef ATTCONTROL_H
#define ATTCONTROL_H

#include <QDialog>

namespace Ui {
class attcontrol;
}

class attcontrol : public QDialog
{
    Q_OBJECT

public:
    explicit attcontrol(QWidget *parent = 0);
    ~attcontrol();

private:
    Ui::attcontrol *ui;
};

#endif // ATTCONTROL_H
