#include "attgrades.h"
#include "ui_attgrades.h"
#include "attcontrol.h"
#include <QSql>
#include <QSqlTableModel>
#include <string>
#include <QString>

int i;
int count;
QSqlDatabase db;

AttGrades::AttGrades(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttGrades)
{
    ui->setupUi(this);
    count = 0;
    /*for (i = 1;i<41;i++)
    ui->comboBox->addItem("اسبوع " + QString::number(i));*/


}

AttGrades::~AttGrades()
{
    delete ui;
}

void AttGrades::on_pushButton_clicked()
{

    QString s = ui->lineEdit->text();
    QSqlTableModel *model = new QSqlTableModel(ui->tableView,db);
    model->setTable("talaba");
    //model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
    model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
    model->setHeaderData(5, Qt::Horizontal, tr("امتحان 1"));
    model->setHeaderData(6, Qt::Horizontal, tr("امتحان 2"));
    model->setHeaderData(7, Qt::Horizontal, tr("امتحان 3"));
    model->setHeaderData(8, Qt::Horizontal, tr("امتحان 4"));
    model->setHeaderData(9, Qt::Horizontal, tr("امتحان 5"));
    model->setHeaderData(10, Qt::Horizontal, tr("امتحان 6"));

   model->submitAll();

   model->setFilter(QString("name like '%%1%'").arg(s));
   for (int i=2;i<5;i++)
   ui->tableView->hideColumn(i);

   for (int i=11;i<21;i++)
   ui->tableView->hideColumn(i);
   ui->tableView->setModel(model);


   QFont font = ui->tableView->horizontalHeader()->font();
   font.setPointSize(12);
   ui->tableView->setFont(font);

   ui->tableView->resizeColumnsToContents();
   ui->tableView->setColumnWidth(2,20);

   //ui->tableView->resizeRowsToContents();
   ui->tableView->verticalHeader()->setVisible(false);
   for (int i=2;i<5;i++)
   ui->tableView->hideColumn(i);

   for (int i=11;i<21;i++)
   ui->tableView->hideColumn(i);

   ui->tableView->show();

   if ( count == 0 )
   {  ui->tableView->horizontalHeader()->swapSections(0,10);
      ui->tableView->horizontalHeader()->swapSections(1,9);
      ui->tableView->horizontalHeader()->swapSections(5,8);
      ui->tableView->horizontalHeader()->swapSections(6,7);
   }

   for (int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c)
   {
       ui->tableView->horizontalHeader()->setSectionResizeMode(
           c, QHeaderView::Stretch);
   }
   count = 1;
}

