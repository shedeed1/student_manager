#include "add.h"
#include "ui_add.h"
#include "mainwindow.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>

bool isCenterGaber;
bool isCenterAhram;

Add::Add(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add)
{
    isCenterGaber = false;
    isCenterAhram = false;
    ui->setupUi(this);
    ui->comboBox->addItem("");
    ui->comboBox->addItem("الجمعة");
    ui->comboBox->addItem("السبت");
    ui->comboBox->addItem("الأحد");
    ui->comboBox->addItem("الأثنين");
    ui->comboBox->addItem("الثلاثاء");
    ui->comboBox->addItem("الأربعاء");
    if (!ui->checkBox->isChecked())
        ui->lineEdit->hide();

}

Add::~Add()
{
    delete ui;
}

void Add::on_pushButton_clicked()
{
       QString id, name, school, grade, number, parent, address, day, time;
       id = ui->lineEdit->text();
       name = ui->lineEdit_2->text();
       school = ui->lineEdit_3->text();
       grade = ui->lineEdit_4->text();
       number = ui->lineEdit_5->text();
       parent = ui->lineEdit_6->text();
       address = ui->lineEdit_7->text();
       day = ui->comboBox->currentText();
       time = ui->comboBox_2->currentText();
       bool isCenterSafwa = ui->checkBox->isChecked();
       bool isCenterGaber = ui->checkBox_2->isChecked();
       bool isCenterAhram = ui->checkBox_3->isChecked();

       QSqlQuery count;
       if (isCenterGaber)
       count.prepare("SELECT id FROM talaba WHERE id LIKE '%G%' ORDER BY rowid DESC");
       else if (ui->checkBox_4->isChecked())
           count.prepare("SELECT id FROM talaba WHERE id LIKE '%R%' ORDER BY rowid DESC");
       count.exec();
       count.first();
       QString intermediate =  count.value(0).toString().remove(0,1);
       int intermediate2 = intermediate.toInt();
       int cnt = intermediate2 + 1;
       QString ss = QString::number(cnt);


       QSqlQuery q;
       q.prepare("INSERT INTO talaba(id,name,grade,number,parent,school,day,time,address) VALUES(:id,:name,:grade,:number,:parent,:school,:day,:time,:address)");
       if (isCenterGaber)
           q.bindValue(":id","G" + ss);
       else if (isCenterAhram)
           q.bindValue(":id","C" + id);
       else if(isCenterSafwa)
       q.bindValue(":id", id);
       else if (ui->checkBox_4->isChecked())
       q.bindValue(":id", "R" + ss);
       else
           q.bindValue(":id", id);

       q.bindValue(":name", name);
       q.bindValue(":school", school);
       q.bindValue(":grade", grade);
       q.bindValue(":number", number);
       q.bindValue(":parent", parent);
       q.bindValue(":address", address);
       q.bindValue(":day",day);
       q.bindValue(":time",time);

       if ( name == "" || day == "" || time == "")
           ui->label_6->setText("تأكد من إدخال جميع البيانات");
       else {
       if(!q.exec())
           qWarning() << "Add - ERROR: " << q.lastError().text();

       ui->lineEdit->clear();
       ui->lineEdit_2->clear();
       ui->lineEdit_3->clear();
       ui->lineEdit_4->clear();
       ui->lineEdit_5->clear();
       ui->lineEdit_6->clear();
       ui->lineEdit_7->clear();
       if (isCenterGaber || ui->checkBox_4->isChecked())
       ui->lineEdit_2->setFocus();
       else
           ui->lineEdit->setFocus();

       ui->label_6->setText("تمت الإضافة");

       emit updatee(); }
}



void Add::on_comboBox_currentIndexChanged(int index)
{
    switch (index)
    {
        case 1:
    {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItem("09:00");
        ui->comboBox_2->addItem("04:00");
        ui->comboBox_2->addItem("06:00");
        break;
    }
    default:
        ui->comboBox_2->clear();
        ui->comboBox_2->addItem("");
        ui->comboBox_2->addItem("09:00");
        ui->comboBox_2->addItem("11:00");
        ui->comboBox_2->addItem("01:00");
        ui->comboBox_2->addItem("03:00");
        ui->comboBox_2->addItem("05:00");
        ui->comboBox_2->addItem("07:00");
        break;
    }
}

void Add::on_checkBox_toggled(bool checked)
{
    if (checked)
    {  if (ui->checkBox_2->isChecked()) ui->checkBox_2->setChecked(false);
        if (ui->checkBox_3->isChecked()) ui->checkBox_3->setChecked(false);
        if (ui->checkBox_4->isChecked()) ui->checkBox_4->setChecked(false);
        ui->lineEdit->show();}
    else
        ui->lineEdit->hide();
}

void Add::on_checkBox_2_toggled(bool checked)
{
    if (checked) { if (ui->checkBox->isChecked()) ui->checkBox->setChecked(false);
        if (ui->checkBox_4->isChecked()) ui->checkBox_4->setChecked(false);
    if (ui->checkBox_3->isChecked()) ui->checkBox_3->setChecked(false);}
}

void Add::on_checkBox_3_toggled(bool checked)
{
    if (checked) { if (ui->checkBox_2->isChecked()) ui->checkBox_2->setChecked(false);
        if (ui->checkBox_4->isChecked()) ui->checkBox_4->setChecked(false);
        ui->lineEdit->show();
    if (ui->checkBox->isChecked()) ui->checkBox->setChecked(false); }
    else
        ui->lineEdit->hide();
}

void Add::on_checkBox_4_toggled(bool checked)
{
    if (checked) { if (ui->checkBox->isChecked()) ui->checkBox->setChecked(false);
    if (ui->checkBox_3->isChecked()) ui->checkBox_3->setChecked(false);
    if (ui->checkBox_2->isChecked()) ui->checkBox_2->setChecked(false);
    }
}
