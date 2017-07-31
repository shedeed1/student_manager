#include "add.h"
#include "ui_add.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>

Add::Add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add)
{
    ui->setupUi(this);
    ui->comboBox->addItem("الجمعة");
    ui->comboBox->addItem("السبت");
    ui->comboBox->addItem("الأحد");
    ui->comboBox->addItem("الأثنين");
    ui->comboBox->addItem("الثلاثاء");
    ui->comboBox->addItem("الأربعاء");
    ui->comboBox->addItem("الخميس");

    ui->comboBox_2->addItem("8:30");
    ui->comboBox_2->addItem("10:00");
    ui->comboBox_2->addItem("01:00");
    ui->comboBox_2->addItem("03:00");
    ui->comboBox_2->addItem("05:00");
    ui->comboBox_2->addItem("07:00");
    ui->comboBox_2->addItem("09:00");
}

Add::~Add()
{
    delete ui;
}

void Add::on_pushButton_clicked()
{
       QString s1,s2, s3, s4, s5;
       s1 = ui->lineEdit->text();
       s2 = ui->lineEdit_2->text();
       s3 = ui->lineEdit_3->text();
       s4 = ui->comboBox->currentText();
       s5 = ui->comboBox_2->currentText();
       QSqlQuery q;
       q.prepare("INSERT INTO talaba(id,name,grade,day,time) VALUES(:id,:name,:grade,:day,:time)");
       q.bindValue(":id",s3);
       q.bindValue(":name", s1);
       q.bindValue(":grade", s2);
       q.bindValue(":day",s4);
       q.bindValue(":time",s5);

       if(!q.exec())
           qWarning() << "Add - ERROR: " << q.lastError().text();

       ui->lineEdit->clear();
       ui->lineEdit_2->clear();

       emit updatee();
}


