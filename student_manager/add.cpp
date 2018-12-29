#include "add.h"
#include "ui_add.h"
#include "mainwindow.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>

Add::Add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add)
{
    ui->setupUi(this);
    ui->comboBox->addItem("");
    ui->comboBox->addItem("السبت");
    ui->comboBox->addItem("الأحد");
    ui->comboBox->addItem("الأثنين");
    ui->comboBox->addItem("الثلاثاء");
    ui->comboBox->addItem("الأربعاء");
}

Add::~Add()
{
    delete ui;
}

void Add::on_pushButton_clicked()
{
       QLocale locale = QLocale(QLocale::Arabic, QLocale::Egypt);
       QDate date2;
       QString id, name, school, grade, number, parent, address, day, time;
       //id = ui->lineEdit->text();
       name = ui->lineEdit_2->text();
       school = ui->lineEdit_3->text();
       grade = ui->lineEdit_4->text();
       number = ui->lineEdit_5->text();
       parent = ui->lineEdit_6->text();
       address = ui->lineEdit_7->text();
       day = ui->comboBox->currentText();
       time = ui->comboBox_2->currentText();

       QSqlQuery q;
       q.prepare("INSERT INTO student(name,grade,number,parent,school,day,time,address,regdate) VALUES(:name,:grade,:number,:parent,:school,:day,:time,:address,:regdate)");

       //q.bindValue(":id", id);
       q.bindValue(":name", name);
       q.bindValue(":school", school);
       q.bindValue(":grade", grade);
       q.bindValue(":number", number);
       q.bindValue(":parent", parent);
       q.bindValue(":address", address);
       q.bindValue(":day",day);
       q.bindValue(":time",time);
       q.bindValue(":regdate",locale.toString(date2.currentDate()));

       if ( name == "" || day == "" || time == "")
           ui->label_6->setText("تأكد من إدخال جميع البيانات");
       else {
       if(!q.exec())
           qWarning() << "Add - ERROR: " << q.lastError().text();


       ui->lineEdit_2->clear();
       ui->lineEdit_3->clear();
       ui->lineEdit_4->clear();
       ui->lineEdit_5->clear();
       ui->lineEdit_6->clear();
       ui->lineEdit_7->clear();

       ui->lineEdit_2->setFocus();


       ui->label_6->setText("تمت الإضافة");

       emit updatee(); }
}



void Add::on_comboBox_currentIndexChanged(int index)
{
    ui->comboBox_2->clear();
    ui->comboBox_2->addItem("");
    ui->comboBox_2->addItem("مجموعة 1");
    ui->comboBox_2->addItem("مجموعة 2");
    ui->comboBox_2->addItem("مجموعة 3");
    ui->comboBox_2->addItem("مجموعة 4");
}
