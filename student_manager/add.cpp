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

    ui->comboBox_2->addItem("08:30");
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
       QString s1,s2, s3, s4, s5, s6, s7, s8;
       s1 = ui->lineEdit->text();
       s2 = ui->lineEdit_2->text();
       s3 = ui->lineEdit_3->text();
       s4 = ui->comboBox->currentText();
       s5 = ui->comboBox_2->currentText();
       s6 = ui->lineEdit_4->text();
       s7 = ui->lineEdit_5->text();
       s8 = ui->lineEdit_6->text();

       QSqlQuery q;
       q.prepare("INSERT INTO talaba(id,name,grade,number,parent,school,day,time) VALUES(:id,:name,:grade,:number,:parent,:school,:day,:time)");
       q.bindValue(":id",s3);
       q.bindValue(":name", s1);
       q.bindValue(":grade", s2);
       q.bindValue(":number", s6);
       q.bindValue(":parent", s7);
       q.bindValue(":school", s8);
       q.bindValue(":day",s4);
       q.bindValue(":time",s5);

       if ( s1 == "" || s3 == "" || s4 == "" || s5 == "")
           ui->label_6->setText("تأكد من إدخال جميع البيانات");
       else {
       if(!q.exec())
           qWarning() << "Add - ERROR: " << q.lastError().text();

       ui->lineEdit->clear();
       ui->lineEdit_2->clear();
       ui->lineEdit_3->clear();
       ui->comboBox->setCurrentIndex(0);
       ui->comboBox_2->setCurrentIndex(0);
       ui->label_6->setText("تمت الإضافة");


       emit updatee(); }
}


