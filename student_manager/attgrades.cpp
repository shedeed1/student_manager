#include "attgrades.h"
#include "ui_attgrades.h"
#include "attcontrol.h"
#include <QSql>
#include <QSqlTableModel>
#include <string>
#include <QString>
#include <QDate>
#include <QDebug>

int i;
int count;
int count2;
bool swapped;
bool swapped2;
QDate friday[40];
QDate saturday[40];
QDate sunday[40];
QDate monday[40];
QDate tuesday[40];
QDate wednesday[40];

QString Sfriday[40];
QString Ssaturday[40];
QString Ssunday[40];
QString Smonday[40];
QString Stuesday[40];
QString Swednesday[40];


AttGrades::AttGrades(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttGrades)
{

    ui->setupUi(this);
    swapped = false;
    swapped2 = false;
    QFont font = ui->tableView->horizontalHeader()->font();
    font.setPointSize(12);
    ui->tableView->setFont(font);
    count = 0;
    ui->comboBox->addItem("");
    ui->comboBox->addItem("الجمعة");
    ui->comboBox->addItem("السبت");
    ui->comboBox->addItem("الأحد");
    ui->comboBox->addItem("الأثنين");
    ui->comboBox->addItem("الثلاثاء");
    ui->comboBox->addItem("الأربعاء");

    ui->comboBox_5->addItem("");
    ui->comboBox_5->addItem("الجمعة");
    ui->comboBox_5->addItem("السبت");
    ui->comboBox_5->addItem("الأحد");
    ui->comboBox_5->addItem("الأثنين");
    ui->comboBox_5->addItem("الثلاثاء");
    ui->comboBox_5->addItem("الأربعاء");

    ui->comboBox_2->addItem("");
    ui->comboBox_2->addItem("09:00");
    ui->comboBox_2->addItem("11:00");
    ui->comboBox_2->addItem("01:00");
    ui->comboBox_2->addItem("03:00");
    ui->comboBox_2->addItem("04:00");
    ui->comboBox_2->addItem("05:00");
    ui->comboBox_2->addItem("06:00");
    ui->comboBox_2->addItem("07:00");
    ui->comboBox_2->addItem("09:00");


    QLocale locale = QLocale(QLocale::Arabic, QLocale::Egypt);
    QDate date;
    date.setDate(2017,8,11);
    int count = 0;
    count2 = 0;

    for (int i =0;i<40;i++)
    {
        friday[i] =  date.addDays(count);
        saturday[i] =  date.addDays(count+1);
        sunday[i] =  date.addDays(count+2);
        monday[i] =  date.addDays(count+3);
        tuesday[i] =  date.addDays(count+4);
        wednesday[i] =  date.addDays(count+5);
        count+=7;
    }
    QString arabicdate;
    for (int i =0;i<40;i++)
    {
        arabicdate = locale.toString(friday[i]);
        Sfriday[i] = arabicdate;

        arabicdate = locale.toString(saturday[i]);
        Ssaturday[i] = arabicdate;

        arabicdate = locale.toString(sunday[i]);
        Ssunday[i] = arabicdate;

        arabicdate = locale.toString(monday[i]);
        Smonday[i] = arabicdate;

        arabicdate = locale.toString(tuesday[i]);
        Stuesday[i] = arabicdate;

        arabicdate = locale.toString(wednesday[i]);
        Swednesday[i] = arabicdate;
    }
}

AttGrades::~AttGrades()
{
    delete ui;
}

void AttGrades::on_pushButton_clicked()
{
    for (int i=0;i<56;i++)
      ui->tableView->showColumn(i);
    QSqlDatabase db;
    QString s = ui->name1->text();
    QSqlTableModel *model = new QSqlTableModel(ui->tableView,db);
    model->setTable("talaba");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);

    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
    model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
    model->setHeaderData(8, Qt::Horizontal, tr("امتحان 1"));
    model->setHeaderData(9, Qt::Horizontal, tr("امتحان 2"));
    model->setHeaderData(10, Qt::Horizontal, tr("امتحان 3"));
    model->setHeaderData(11, Qt::Horizontal, tr("امتحان 4"));
    model->setHeaderData(12, Qt::Horizontal, tr("امتحان 5"));
    model->setHeaderData(13, Qt::Horizontal, tr("امتحان 6"));


   if (ui->comboBox->currentText()=="" && ui->comboBox_2->currentText()=="") {
   if (ui->id1->text() == "" && ui->name1->text() != "")
   model->setFilter(QString("name like '%%1%'").arg(s));
   else if (ui->name1->text() == "" && ui->id1->text() != "")
    model->setFilter(QString("id like '%1%'").arg(ui->id1->text()));
   else if (ui->name1->text() != "" && ui->id1->text() != "")
       model->setFilter(QString("name like '%%1%' AND id like '%2%'").arg(s).arg(ui->id1->text() != ""));
   else if (ui->name1->text() == "" && ui->id1->text() == "")
   {} }

   else if (ui->comboBox->currentText()!="" && ui->comboBox_2->currentText()=="")
   {
       if (ui->id1->text() == "" && ui->name1->text() != "")
       model->setFilter(QString("name like '%%1%' AND day like '%%2%'").arg(s).arg(ui->comboBox->currentText()));
       else if (ui->name1->text() == "" && ui->id1->text() != "")
        model->setFilter(QString("id like '%1%' AND day like '%%2%'").arg(ui->id1->text()).arg(ui->comboBox->currentText()));
       else if (ui->name1->text() != "" && ui->id1->text() != "")
           model->setFilter(QString("name like '%%1%' AND id like '%2%' AND day like '%%3%'").arg(s).arg(ui->id1->text()).arg(ui->comboBox->currentText()));
       else if (ui->name1->text() == "" && ui->id1->text() == "")
       {model->setFilter(QString("day like '%%1%'").arg(ui->comboBox->currentText()));}
   }

   else if (ui->comboBox->currentText()=="" && ui->comboBox_2->currentText()!="")
   {
       if (ui->id1->text() == "" && ui->name1->text() != "")
       model->setFilter(QString("name like '%%1%' AND time like '%%2%'").arg(s).arg(ui->comboBox_2->currentText()));
       else if (ui->name1->text() == "" && ui->id1->text() != "")
        model->setFilter(QString("id like '%1%' AND time like '%%2%'").arg(ui->id1->text()).arg(ui->comboBox_2->currentText()));
       else if (ui->name1->text() != "" && ui->id1->text() != "")
           model->setFilter(QString("name like '%%1%' AND id like '%2%' AND time like '%%3%'").arg(s).arg(ui->id1->text()).arg(ui->comboBox_2->currentText()));
       else if (ui->name1->text() == "" && ui->id1->text() == "")
       {model->setFilter(QString("time like '%%1%'").arg(ui->comboBox_2->currentText()));}
   }

   else if (ui->comboBox->currentText()!="" && ui->comboBox_2->currentText()!="")
   {
       if (ui->id1->text() == "" && ui->name1->text() != "")
       model->setFilter(QString("name like '%%1%' AND day like '%%2' AND time like '%%3%'").arg(s).arg(ui->comboBox->currentText()).arg(ui->comboBox_2->currentText()));
       else if (ui->name1->text() == "" && ui->id1->text() != "")
        model->setFilter(QString("id like '%1%' AND day like '%%2%' AND time like '%%3'").arg(ui->id1->text()).arg(ui->comboBox->currentText()).arg(ui->comboBox_2->currentText()));
       else if (ui->name1->text() != "" && ui->id1->text() != "")
           model->setFilter(QString("name like '%%1%' AND id like '%2%' AND day like '%%3%' AND time like '%%4%'").arg(s).arg(ui->id1->text()).arg(ui->comboBox->currentText()).arg(ui->comboBox_2->currentText()));
       else if (ui->name1->text() == "" && ui->id1->text() == "")
       {model->setFilter(QString("day like '%%1%' AND time like '%%2%'").arg(ui->comboBox->currentText()).arg(ui->comboBox_2->currentText()));}
   }

   ui->tableView->setModel(model);

   QFont font = ui->tableView->horizontalHeader()->font();
   font.setPointSize(12);
   ui->tableView->setFont(font);

   ui->tableView->resizeColumnsToContents();
   ui->tableView->resizeRowsToContents();
   ui->tableView->verticalHeader()->setVisible(false);

   for (int i=2;i<8;i++)
   ui->tableView->hideColumn(i);

   for (int i=14;i<56;i++)
   ui->tableView->hideColumn(i);

   ui->tableView->show();

   if ( count == 0 )
   {
       if (!swapped) {
      ui->tableView->horizontalHeader()->swapSections(0,53);
      ui->tableView->horizontalHeader()->swapSections(1,52); }
      ui->tableView->horizontalHeader()->swapSections(8,51);
      ui->tableView->horizontalHeader()->swapSections(9,50);
      ui->tableView->horizontalHeader()->swapSections(10,49);
      ui->tableView->horizontalHeader()->swapSections(11,48);
      ui->tableView->horizontalHeader()->swapSections(12,47);
      ui->tableView->horizontalHeader()->swapSections(13,46);

   }
   swapped2 = true;


   for (int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c)
   {
       ui->tableView->horizontalHeader()->setSectionResizeMode(
           c, QHeaderView::Stretch);
   }
   count = 1;
}

void AttGrades::on_comboBox_5_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        ui->comboBox_6->clear();
        ui->comboBox_7->clear();
        break;
    case 1:
        ui->comboBox_7->clear();
        ui->comboBox_7->addItem("");
        ui->comboBox_7->addItem("09:00");
        ui->comboBox_7->addItem("04:00");
        ui->comboBox_7->addItem("06:00");
        ui->comboBox_6->clear();
        for (int i = 0;i<40;i++)
        ui->comboBox_6->addItem(Sfriday[i]);
        break;
    case 2:
        ui->comboBox_7->clear();
        ui->comboBox_7->addItem("");
        ui->comboBox_7->addItem("09:00");
        ui->comboBox_7->addItem("11:00");
        ui->comboBox_7->addItem("01:00");
        ui->comboBox_7->addItem("03:00");
        ui->comboBox_7->addItem("05:00");
        ui->comboBox_7->addItem("07:00");
        ui->comboBox_6->clear();
        for (int i = 0;i<40;i++)
        ui->comboBox_6->addItem(Ssaturday[i]);
        break;
    case 3:
        ui->comboBox_7->clear();
        ui->comboBox_7->addItem("");
        ui->comboBox_7->addItem("09:00");
        ui->comboBox_7->addItem("11:00");
        ui->comboBox_7->addItem("01:00");
        ui->comboBox_7->addItem("03:00");
        ui->comboBox_7->addItem("05:00");
        ui->comboBox_7->addItem("07:00");
        ui->comboBox_6->clear();
        for (int i = 0;i<40;i++)
        ui->comboBox_6->addItem(Ssunday[i]);
        break;
    case 4:
        ui->comboBox_7->clear();
        ui->comboBox_7->addItem("");
        ui->comboBox_7->addItem("09:00");
        ui->comboBox_7->addItem("11:00");
        ui->comboBox_7->addItem("01:00");
        ui->comboBox_7->addItem("03:00");
        ui->comboBox_7->addItem("05:00");
        ui->comboBox_7->addItem("07:00");
        ui->comboBox_6->clear();
        for (int i = 0;i<40;i++)
        ui->comboBox_6->addItem(Smonday[i]);
        break;
    case 5:
        ui->comboBox_7->clear();
        ui->comboBox_7->addItem("");
        ui->comboBox_7->addItem("09:00");
        ui->comboBox_7->addItem("11:00");
        ui->comboBox_7->addItem("01:00");
        ui->comboBox_7->addItem("03:00");
        ui->comboBox_7->addItem("05:00");
        ui->comboBox_7->addItem("07:00");
        ui->comboBox_6->clear();
        for (int i = 0;i<40;i++)
        ui->comboBox_6->addItem(Stuesday[i]);
        break;
    case 6:
        ui->comboBox_7->clear();
        ui->comboBox_7->addItem("");
        ui->comboBox_7->addItem("09:00");
        ui->comboBox_7->addItem("11:00");
        ui->comboBox_7->addItem("01:00");
        ui->comboBox_7->addItem("03:00");
        ui->comboBox_7->addItem("05:00");
        ui->comboBox_7->addItem("07:00");
        ui->comboBox_6->clear();
        for (int i = 0;i<40;i++)
        ui->comboBox_6->addItem(Swednesday[i]);
        break;
    }
}

void AttGrades::on_pushButton_2_clicked() // Show attendance
{
    QString s = ui->comboBox_5->currentText();
    QString s1 = QString::number(ui->comboBox_6->currentIndex());
    QString s2 = ui->comboBox_7->currentText();
    int data = ui->comboBox_6->currentIndex();

    QSqlDatabase db;
    QSqlTableModel *model = new QSqlTableModel(ui->tableView,db);

    if (data!= -1 || s != "") {

     for (int i=0;i<56;i++)
       ui->tableView->showColumn(i);

    ui->label_2->clear();

    qDebug() << s1;

    model->setTable("talaba");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);

    model->select();

    model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
    model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
    model->setHeaderData(14+data, Qt::Horizontal, tr("حــضــور"));

   if (ui->id2->text() == "" && ui->name2->text() == "" && s2 == "")
   model->setFilter(QString("day like '%%1%'").arg(s));
   else
   if (ui->id2->text() != "" && ui->name2->text() == "" && s2 == "")
   model->setFilter(QString("day like '%%1%' AND id like '%2%'").arg(s).arg(ui->id2->text()));

   else
   if (ui->id2->text() == "" && ui->name2->text() != "" && s2 == "")
   model->setFilter(QString("day like '%%1%' AND name like '%2%'").arg(s).arg(ui->name2->text()));

   else
   if (ui->id2->text() == "" && ui->name2->text() == "" && s2 != "")
   model->setFilter(QString("day like '%%1%' AND time like '%2%'").arg(s).arg(s2));

   else
   if (ui->id2->text() == "" && ui->name2->text() != "" && s2 != "")
    model->setFilter(QString("day like '%%1%' AND time like '%2%' AND name like '%%3%'").arg(s).arg(s2).arg(ui->name2->text()));

   else
   if (ui->id2->text() != "" && ui->name2->text() == "" && s2 != "")
    model->setFilter(QString("day like '%%1%' AND time like '%2%' AND id like '%3%'").arg(s).arg(s2).arg(ui->id2->text()));

   else
   if (ui->id2->text() != "" && ui->name2->text() != "" && s2 == "")
    model->setFilter(QString("day like '%%1%' AND name like '%%2%' AND id like '%3%'").arg(s).arg(ui->name2->text()).arg(ui->id2->text()));

   else
   if (ui->id2->text() != "" && ui->name2->text() != "" && s2 != "")
   model->setFilter(QString("day like '%%1%' AND name like '%%2%' AND id like '%3%' AND time like '%4%'").arg(s).arg(ui->name2->text()).arg(ui->id2->text()).arg(s2));

   ui->tableView->setModel(model);
   if (count2 == 0)
   {
       if (!swapped2) {
   ui->tableView->horizontalHeader()->swapSections(0,53);
   ui->tableView->horizontalHeader()->swapSections(1,52); }
   }
   swapped = true;
   count2 = 1;

   for (int i=2;i<56;i++)
   if (i!=(14+ui->comboBox_6->currentIndex()))
       ui->tableView->hideColumn(i);
   for (int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c)
   {
       ui->tableView->horizontalHeader()->setSectionResizeMode(
           c, QHeaderView::Stretch);
   }
   //ui->tableView->verticalHeader()->setVisible(false);
   //ui->tableView->show();
   }

    else

    {
        ui->label_2->setText("ادخل اليوم لعرض وتعديل الحضور"); }
}


