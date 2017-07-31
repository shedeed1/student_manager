#include "attgrades.h"
#include "ui_attgrades.h"

AttGrades::AttGrades(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttGrades)
{
    ui->setupUi(this);
}

AttGrades::~AttGrades()
{
    delete ui;
}
