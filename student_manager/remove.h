#ifndef REMOVE_H
#define REMOVE_H

#include <QDialog>

namespace Ui {
class Remove;
}

class Remove : public QDialog
{
    Q_OBJECT

public:
    explicit Remove(QWidget *parent = 0);
    ~Remove();

private slots:
    void on_pushButton_clicked();

signals:
    void updatee();

private:
    Ui::Remove *ui;
};

#endif // REMOVE_H
