#include "studentinfo.h"
#include "ui_studentinfo.h"

StudentInfo::StudentInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentInfo)
{
    ui->setupUi(this);
}

void StudentInfo::setData(const QString &labelText, const QString &labelText2) {
  ui->label_3->setText(labelText);
  ui->label_4->setText(labelText2);
}

StudentInfo::~StudentInfo()
{
    delete ui;
}
