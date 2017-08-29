// Developed by Mohamed Shedeed

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "add.h"
#include "attgrades.h"
#include "remove.h"
#include "StudentInfo.h"
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
QNetworkReply *reply;

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
       model->setHeaderData(54, Qt::Horizontal, tr("منطقة السكن"));
       model->setHeaderData(55, Qt::Horizontal, tr("ملاحظات"));

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
    model->setHeaderData(54, Qt::Horizontal, tr("منطقة السكن"));
    model->setHeaderData(55, Qt::Horizontal, tr("ملاحظات"));

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
    ui->tableView->horizontalHeader()->swapSections(54,45);
    ui->tableView->horizontalHeader()->swapSections(55,44);
    for (int i=8;i<54;i++)
    ui->tableView->hideColumn(i);

    ui->tableView->resizeColumnsToContents();
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->show();
}

void MainWindow::updateTbl()
{
    for (int i=0;i<56;i++)
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
    model->setHeaderData(54, Qt::Horizontal, tr("منطقة السكن"));
    model->setHeaderData(55, Qt::Horizontal, tr("ملاحظات"));

    while(model->canFetchMore()) {model->fetchMore();}

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
       ui->tableView->horizontalHeader()->swapSections(54,45);
       ui->tableView->horizontalHeader()->swapSections(55,44);
       first = false;
   }



   for (int i=8;i<54;i++)
   ui->tableView->hideColumn(i);

   ui->tableView->verticalHeader()->setVisible(false);
   ui->tableView->show();
   ui->tableView->scrollToBottom();
}

void MainWindow::DatabaseInit()
{
    if (db.tables().contains(QLatin1String("talaba"))) {first = false;}
    else { first = true;
    QSqlQuery query("CREATE TABLE talaba (id TEXT, name TEXT, grade TEXT, number TEXT, parent TEXT, school TEXT, day TEXT, time TEXT, exam1 TEXT, exam2 TEXT, exam3 TEXT, exam4 TEXT, exam5 TEXT, exam6 TEXT, day1 TEXT, day2 TEXT, day3 TEXT, day4 TEXT, day5 TEXT, day6 TEXT, day7 TEXT, day8 TEXT, day9 TEXT, day10 TEXT, day11 TEXT, day12 TEXT, day13 TEXT, day14 TEXT, day15 TEXT, day16 TEXT, day17 TEXT, day18 TEXT, day19 TEXT, day20 TEXT, day21 TEXT, day22 TEXT, day23 TEXT, day24 TEXT, day25 TEXT, day26 TEXT, day27 TEXT, day28 TEXT, day29 TEXT, day30 TEXT, day31 TEXT, day32 TEXT, day33 TEXT, day34 TEXT, day35 TEXT, day36 TEXT, day37 TEXT, day38 TEXT, day39 TEXT, day40 TEXT, address TEXT, notes TEXT)");

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

void MainWindow::upload()
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-sqlite3"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"db\"; filename=\"1.db\""));
    QFile *file = new QFile("./talaba.db");
    file->open(QIODevice::ReadOnly);
    imagePart.setBodyDevice(file);
    file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

    multiPart->append(imagePart);

    QUrl url("http://shedeed.5gbfree.com/index.php/Upload/do_upload/");
    QNetworkRequest request(url);

    QNetworkAccessManager *manager= new QNetworkAccessManager;
    reply = manager->post(request, multiPart);
    multiPart->setParent(reply); // delete the multiPart with the reply
    // here connect signals etc.
    connect(reply, SIGNAL(finished()),
                  this, SLOT  (uploadDone()));

         connect(reply, SIGNAL(uploadProgress(qint64, qint64)),
                  this, SLOT  (uploadProgress(qint64, qint64)));
}

void MainWindow::uploadProgress(qint64 bytesSent, qint64 bytesTotal) {
    qDebug() << "---------Uploaded--------------" << bytesSent<< "of" <<bytesTotal;
}

void MainWindow::uploadDone() {
    qDebug() << "----------Finished--------------" << reply->errorString() <<reply->attribute( QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<reply;


}

void MainWindow::on_comboBox_currentIndexChanged(int index)
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

void MainWindow::on_pushButton_6_clicked()
{
    upload();
}


void MainWindow::on_pushButton_7_clicked()
{
    int count = 0;
    QItemSelectionModel *select = ui->tableView->selectionModel();
    if ( select->currentIndex().column() == 1 ) {
    QString selection = ui->tableView->model()->data(select->currentIndex()).toString();

    QSqlQuery attendance;
    attendance.prepare("SELECT * FROM talaba where name = ?");
    attendance.addBindValue(selection);
    attendance.exec();
    attendance.first();

    for (int i =0;i<56;i++)
        if (attendance.value(i).toString() == "غ")
            count++;


    StudentInfo stdnfo;
    stdnfo.setModal(true);
    stdnfo.setData(selection,QString::number(count));
    stdnfo.exec(); }
}
