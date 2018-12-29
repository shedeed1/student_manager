#ifndef ADD_H
#define ADD_H

#include <QDialog>
#include <mainwindow.h>

namespace Ui {
class Add;
}

class Add : public QDialog
{
    Q_OBJECT

public:
    explicit Add(QWidget *parent = 0);
    ~Add();

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

signals:
    void updatee();

private:
    Ui::Add *ui;
};

#endif // ADD_H
