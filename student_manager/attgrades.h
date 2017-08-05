#ifndef ATTGRADES_H
#define ATTGRADES_H

#include <QDialog>

namespace Ui {
class AttGrades;
}

class AttGrades : public QDialog
{
    Q_OBJECT

public:
    explicit AttGrades(QWidget *parent = 0);
    ~AttGrades();

private slots:
    void on_pushButton_clicked();

private:
    Ui::AttGrades *ui;
};

#endif // ATTGRADES_H
