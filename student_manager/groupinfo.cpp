#include "groupinfo.h"
#include "ui_groupinfo.h"

groupinfo::groupinfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::groupinfo)
{
    ui->setupUi(this);
}

void groupinfo::setData(const QString &groupNumber, const QString &day, const QString &date, const QString &attended, const QString &missed, const QString &comp)
{
    ui->groupNumber->setText(groupNumber);
    ui->day->setText(day);
    ui->date->setText(date);

    ui->attendanceLabel->setText(ui->attendanceLabel->text()+" " +attended);
    ui->missedLabel->setText(ui->missedLabel->text()+" " +missed);
    ui->compLabel->setText(ui->compLabel->text()+" " +comp);

}

groupinfo::~groupinfo()
{
    delete ui;
}
