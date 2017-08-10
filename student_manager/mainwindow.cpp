// Developed by Mohamed Shedeed

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "add.h"
#include "attgrades.h"
#include "remove.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileInfo>
#include <QTableView>
#include <QSqlTableModel>
#include <QDialog>
#include <QFile>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QSqlDatabase db;
bool first;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    first = true;
    ui->setupUi(this);
    ui->comboBox->addItem("");
    ui->comboBox->addItem("الجمعة");
    ui->comboBox->addItem("السبت");
    ui->comboBox->addItem("الأحد");
    ui->comboBox->addItem("الأثنين");
    ui->comboBox->addItem("الثلاثاء");
    ui->comboBox->addItem("الأربعاء");

    ui->comboBox_2->addItem("");
    ui->comboBox_2->addItem("08:30");
    ui->comboBox_2->addItem("10:00");
    ui->comboBox_2->addItem("01:00");
    ui->comboBox_2->addItem("03:00");
    ui->comboBox_2->addItem("05:00");
    ui->comboBox_2->addItem("07:00");
    ui->comboBox_2->addItem("09:00");

    DatabaseConnect(); // Connecting to database and organizing table

    DatabaseInit(); // Initializing database
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
    Remove remove;
    connect(&remove,SIGNAL(updatee()),this,SLOT(updateTbl()));
    remove.setModal(true);
    remove.exec();
}

void MainWindow::on_pushButton_3_clicked() // Search
{
       QString s;
       s = ui->lineEdit->text();
       QSqlTableModel *model = new QSqlTableModel(ui->tableView,db);
       model->setTable("talaba");
       model->setEditStrategy(QSqlTableModel::OnFieldChange);
       model->select();
       model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
       model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
       model->setHeaderData(2, Qt::Horizontal, tr("مجموع الاعدادية"));
       model->setHeaderData(3, Qt::Horizontal, tr("رقم الهاتف"));
       model->setHeaderData(4, Qt::Horizontal, tr("رقم ولي الأمر"));
       model->setHeaderData(5, Qt::Horizontal, tr("المدرسة"));
       model->setHeaderData(6, Qt::Horizontal, tr("اليوم"));
       model->setHeaderData(7, Qt::Horizontal, tr("الساعة"));

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
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
    model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
    model->setHeaderData(2, Qt::Horizontal, tr("مجموع الاعدادية"));
    model->setHeaderData(3, Qt::Horizontal, tr("رقم الهاتف"));
    model->setHeaderData(4, Qt::Horizontal, tr("رقم ولي الأمر"));
    model->setHeaderData(5, Qt::Horizontal, tr("المدرسة"));
    model->setHeaderData(6, Qt::Horizontal, tr("اليوم"));
    model->setHeaderData(7, Qt::Horizontal, tr("الساعة"));

    ui->tableView->setModel(model);

    for (int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c)
    {
        ui->tableView->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }

    QFont font = ui->tableView->horizontalHeader()->font();
    font.setPointSize(12);
    ui->tableView->setFont(font);

    ui->tableView->horizontalHeader()->swapSections(0,53);
    ui->tableView->horizontalHeader()->swapSections(1,52);
    ui->tableView->horizontalHeader()->swapSections(2,51);
    ui->tableView->horizontalHeader()->swapSections(3,50);
    ui->tableView->horizontalHeader()->swapSections(4,49);
    ui->tableView->horizontalHeader()->swapSections(5,48);
    ui->tableView->horizontalHeader()->swapSections(6,47);
    ui->tableView->horizontalHeader()->swapSections(7,46);
    for (int i=8;i<54;i++)
    ui->tableView->hideColumn(i);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->setColumnWidth(2,20);
    //ui->tableView->resizeRowsToContents();
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->show();
}

void MainWindow::updateTbl()
{
    for (int i=0;i<54;i++)
      ui->tableView->showColumn(i);
    QSqlTableModel *model = new QSqlTableModel(ui->tableView,db);
    model->setTable("talaba");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
    model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
    model->setHeaderData(2, Qt::Horizontal, tr("مجموع الاعدادية"));
    model->setHeaderData(3, Qt::Horizontal, tr("رقم الهاتف"));
    model->setHeaderData(4, Qt::Horizontal, tr("رقم ولي الأمر"));
    model->setHeaderData(5, Qt::Horizontal, tr("المدرسة"));
    model->setHeaderData(6, Qt::Horizontal, tr("اليوم"));
    model->setHeaderData(7, Qt::Horizontal, tr("الساعة"));

   ui->tableView->setModel(model);

   QFont font = ui->tableView->horizontalHeader()->font();
   font.setPointSize(12);
   ui->tableView->setFont(font);

   if (first)
   {
       ui->tableView->horizontalHeader()->swapSections(0,53);
       ui->tableView->horizontalHeader()->swapSections(1,52);
       ui->tableView->horizontalHeader()->swapSections(2,51);
       ui->tableView->horizontalHeader()->swapSections(3,50);
       ui->tableView->horizontalHeader()->swapSections(4,49);
       ui->tableView->horizontalHeader()->swapSections(5,48);
       ui->tableView->horizontalHeader()->swapSections(6,47);
       ui->tableView->horizontalHeader()->swapSections(7,46);
       first = false;
   }

   for (int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c)
   {
       ui->tableView->horizontalHeader()->setSectionResizeMode(
           c, QHeaderView::Stretch);
   }

   for (int i=8;i<54;i++)
   ui->tableView->hideColumn(i);


   ui->tableView->verticalHeader()->setVisible(false);
   ui->tableView->show();
}

void MainWindow::DatabaseInit()
{
    if (db.tables().contains(QLatin1String("talaba"))) {first = false;}
    else { first = true;
    QSqlQuery query("CREATE TABLE talaba (id TEXT, name TEXT, grade TEXT, number TEXT, parent TEXT, school TEXT, day TEXT, time TEXT, exam1 TEXT, exam2 TEXT, exam3 TEXT, exam4 TEXT, exam5 TEXT, exam6 TEXT, day1 TEXT, day2 TEXT, day3 TEXT, day4 TEXT, day5 TEXT, day6 TEXT, day7 TEXT, day8 TEXT, day9 TEXT, day10 TEXT, day11 TEXT, day12 TEXT, day13 TEXT, day14 TEXT, day15 TEXT, day16 TEXT, day17 TEXT, day18 TEXT, day19 TEXT, day20 TEXT, day21 TEXT, day22 TEXT, day23 TEXT, day24 TEXT, day25 TEXT, day26 TEXT, day27 TEXT, day28 TEXT, day29 TEXT, day30 TEXT, day31 TEXT, day32 TEXT, day33 TEXT, day34 TEXT, day35 TEXT, day36 TEXT, day37 TEXT, day38 TEXT, day39 TEXT, day40 TEXT)");

    if(!query.isActive())
        qWarning() << "MainWindow::DatabaseInit - ERROR: " << query.lastError().text(); }
}


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
    attgrdz.setModal(true);
    attgrdz.exec();
}


