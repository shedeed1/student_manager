#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "add.h"
#include "attgrades.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileInfo>
#include <QTableView>
#include <QSqlTableModel>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("");
    ui->comboBox->addItem("الجمعة");
    ui->comboBox->addItem("السبت");
    ui->comboBox->addItem("الأحد");
    ui->comboBox->addItem("الأثنين");
    ui->comboBox->addItem("الثلاثاء");
    ui->comboBox->addItem("الأربعاء");
    ui->comboBox->addItem("الخميس");

    ui->comboBox_2->addItem("");
    ui->comboBox_2->addItem("08:30");
    ui->comboBox_2->addItem("10:00");
    ui->comboBox_2->addItem("01:00");
    ui->comboBox_2->addItem("03:00");
    ui->comboBox_2->addItem("05:00");
    ui->comboBox_2->addItem("07:00");
    ui->comboBox_2->addItem("09:00");
    DatabaseConnect();
    QFileInfo check_file("./talaba.db");
    if (check_file.exists()) {

        } else {
            DatabaseInit(); }

    DatabaseInit();

    // PROBLEM HERE
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked() // Add
{
   Add add;
   connect(&add,SIGNAL(updatee()),this,SLOT(updateTbl()));
   add.setModal(true);
   add.exec();
}

void MainWindow::on_pushButton_2_clicked() // Remove
{

}

void MainWindow::on_pushButton_3_clicked() // Search
{
       QString s;
       s = ui->lineEdit->text();
       QSqlDatabase db;
       QSqlTableModel *model = new QSqlTableModel(ui->tableView,db);
       model->setTable("talaba");
       //model->setEditStrategy(QSqlTableModel::OnManualSubmit);
       model->select();
       model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
       model->setHeaderData(2, Qt::Horizontal, tr("مجموع الاعدادية"));
       model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
       model->setHeaderData(3, Qt::Horizontal, tr("اليوم"));
       model->setHeaderData(4, Qt::Horizontal, tr("الساعة"));

      model->submitAll();

      if (ui->comboBox->currentText()=="" && ui->comboBox_2->currentText()=="") {
      if (ui->lineEdit_2->text() == "" && ui->lineEdit->text() != "")
      model->setFilter(QString("name like '%%1%'").arg(s));
      else if (ui->lineEdit->text() == "" && ui->lineEdit_2->text() != "")
       model->setFilter(QString("id like '%1%'").arg(ui->lineEdit_2->text()));
      else if (ui->lineEdit->text() != "" && ui->lineEdit_2->text() != "")
          model->setFilter(QString("name like '%%1%' AND id like '%2%'").arg(s).arg(ui->lineEdit_2->text() != ""));
      else if (ui->lineEdit->text() == "" && ui->lineEdit_2->text() == "")
      {} }

      else if (ui->comboBox->currentText()!="" && ui->comboBox_2->currentText()=="")
      {
          if (ui->lineEdit_2->text() == "" && ui->lineEdit->text() != "")
          model->setFilter(QString("name like '%%1%' AND day like '%%2%'").arg(s).arg(ui->comboBox->currentText()));
          else if (ui->lineEdit->text() == "" && ui->lineEdit_2->text() != "")
           model->setFilter(QString("id like '%1%' AND day like '%%2%'").arg(ui->lineEdit_2->text()).arg(ui->comboBox->currentText()));
          else if (ui->lineEdit->text() != "" && ui->lineEdit_2->text() != "")
              model->setFilter(QString("name like '%%1%' AND id like '%2%' AND day like '%%3%'").arg(s).arg(ui->lineEdit_2->text()).arg(ui->comboBox->currentText()));
          else if (ui->lineEdit->text() == "" && ui->lineEdit_2->text() == "")
          {model->setFilter(QString("day like '%%1%'").arg(ui->comboBox->currentText()));}
      }

      else if (ui->comboBox->currentText()=="" && ui->comboBox_2->currentText()!="")
      {
          if (ui->lineEdit_2->text() == "" && ui->lineEdit->text() != "")
          model->setFilter(QString("name like '%%1%' AND time like '%%2%'").arg(s).arg(ui->comboBox_2->currentText()));
          else if (ui->lineEdit->text() == "" && ui->lineEdit_2->text() != "")
           model->setFilter(QString("id like '%1%' AND time like '%%2%'").arg(ui->lineEdit_2->text()).arg(ui->comboBox_2->currentText()));
          else if (ui->lineEdit->text() != "" && ui->lineEdit_2->text() != "")
              model->setFilter(QString("name like '%%1%' AND id like '%2%' AND time like '%%3%'").arg(s).arg(ui->lineEdit_2->text()).arg(ui->comboBox_2->currentText()));
          else if (ui->lineEdit->text() == "" && ui->lineEdit_2->text() == "")
          {model->setFilter(QString("time like '%%1%'").arg(ui->comboBox_2->currentText()));}
      }

      else if (ui->comboBox->currentText()!="" && ui->comboBox_2->currentText()!="")
      {
          if (ui->lineEdit_2->text() == "" && ui->lineEdit->text() != "")
          model->setFilter(QString("name like '%%1%' AND day like '%%2' AND time like '%%3%'").arg(s).arg(ui->comboBox->currentText()).arg(ui->comboBox_2->currentText()));
          else if (ui->lineEdit->text() == "" && ui->lineEdit_2->text() != "")
           model->setFilter(QString("id like '%1%' AND day like '%%2%' AND time like '%%3'").arg(ui->lineEdit_2->text()).arg(ui->comboBox->currentText()).arg(ui->comboBox_2->currentText()));
          else if (ui->lineEdit->text() != "" && ui->lineEdit_2->text() != "")
              model->setFilter(QString("name like '%%1%' AND id like '%2%' AND day like '%%3%' AND time like '%%4%'").arg(s).arg(ui->lineEdit_2->text()).arg(ui->comboBox->currentText()).arg(ui->comboBox_2->currentText()));
          else if (ui->lineEdit->text() == "" && ui->lineEdit_2->text() == "")
          {model->setFilter(QString("day like '%%1%' AND time like '%%2%'").arg(ui->comboBox->currentText()).arg(ui->comboBox_2->currentText()));}
      }

      ui->tableView->setModel(model);
}

void MainWindow::DatabaseConnect()
{
    const QString DRIVER("QSQLITE");

    QSqlDatabase db;

    if(QSqlDatabase::isDriverAvailable(DRIVER))
    {
        db = QSqlDatabase::addDatabase(DRIVER);

        db.setDatabaseName("./talaba.db");

        if(!db.open())
            qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
    }
    else
        qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";

    QSqlTableModel *model = new QSqlTableModel(ui->tableView,db);
    model->setTable("talaba");
    //model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
    model->setHeaderData(2, Qt::Horizontal, tr("مجموع الاعدادية"));
    model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
    model->setHeaderData(3, Qt::Horizontal, tr("اليوم"));
    model->setHeaderData(4, Qt::Horizontal, tr("الساعة"));

    model->submitAll();

    ui->tableView->setModel(model);

    for (int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c)
    {
        ui->tableView->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }

    QFont font = ui->tableView->horizontalHeader()->font();
    font.setPointSize(12);
    ui->tableView->setFont(font);

    ui->tableView->horizontalHeader()->moveSection(0,4);
    ui->tableView->horizontalHeader()->moveSection(0,3);
    ui->tableView->horizontalHeader()->moveSection(0,2);
    for (int i=5;i<21;i++)
    ui->tableView->hideColumn(i);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnWidth(2,20);
    //ui->tableView->resizeRowsToContents();
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->show();
}

void MainWindow::updateTbl()
{
    QSqlDatabase db;
    QSqlTableModel *model = new QSqlTableModel(ui->tableView,db);
    model->setTable("talaba");
    //model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();
    model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
    model->setHeaderData(2, Qt::Horizontal, tr("مجموع الاعدادية"));
    model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
    model->setHeaderData(3, Qt::Horizontal, tr("اليوم"));
     model->setHeaderData(4, Qt::Horizontal, tr("الساعة"));

   model->submitAll();

   ui->tableView->setModel(model);

   for (int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c)
   {
       ui->tableView->horizontalHeader()->setSectionResizeMode(
           c, QHeaderView::Stretch);
   }

   QFont font = ui->tableView->horizontalHeader()->font();
   font.setPointSize(12);
   ui->tableView->setFont(font);

   ui->tableView->resizeColumnsToContents();
   ui->tableView->setColumnWidth(2,20);
   //ui->tableView->resizeRowsToContents();
   ui->tableView->verticalHeader()->setVisible(false);
   ui->tableView->show();
}

void MainWindow::DatabaseInit()
{
    QSqlQuery query("CREATE TABLE talaba (id TEXT, name TEXT, grade TEXT, day TEXT, time TEXT, exam1 TEXT, exam2 TEXT, exam3 TEXT, exam4 TEXT, exam5 TEXT, exam6 TEXT, day1 BOOLEAN, day2 BOOLEAN, day3 BOOLEAN, day4 BOOLEAN, day5 BOOLEAN, day6 BOOLEAN, day7 BOOLEAN, day8 BOOLEAN, day9 BOOLEAN, day10 BOOLEAN)");

    if(!query.isActive())
        qWarning() << "MainWindow::DatabaseInit - ERROR: " << query.lastError().text();
}


/*void MainWindow::DatabasePopulate()
{
    QSqlQuery query;

    if(!query.exec("INSERT INTO talaba(name) VALUES('مصطفى محمود')"))
        qWarning() << "MainWindow::DatabasePopulate - ERROR: " << query.lastError().text();
}*/


void MainWindow::on_pushButton_4_clicked() // Reset
{
    updateTbl();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
}

void MainWindow::on_pushButton_5_clicked() // Attendance and grades
{
    AttGrades attgrdz;
    //connect(&add,SIGNAL(updatee()),this,SLOT(updateTbl()));
    attgrdz.setModal(true);
    attgrdz.exec();
}
