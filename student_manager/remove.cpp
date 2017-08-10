#include "remove.h"
#include "ui_remove.h"
#include <QtSql>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

Remove::Remove(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Remove)
{
    ui->setupUi(this);
}

Remove::~Remove()
{
    delete ui;
}

void Remove::on_pushButton_clicked()
{
    QString s = ui->lineEdit->text();
    QSqlQuery check;
    check.prepare("SELECT count(*) FROM talaba where id = ?");
    check.addBindValue(s);
    check.exec();
    check.first();
    QSqlQuery q;
    q.prepare("DELETE FROM talaba WHERE id = ?");
    q.addBindValue(s);

    if(!q.exec() || check.value(0) == 0) {
        qWarning() << "Remove - ERROR: " << q.lastError().text();
        ui->label_2->setText("لم يتم الحذف, تأكد من ادخال رقم صحيح"); }
    else {
        ui->lineEdit->clear();
        ui->label_2->setText("تـم الحـذف");
        emit updatee();
    }
}
