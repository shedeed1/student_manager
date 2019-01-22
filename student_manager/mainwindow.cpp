// Developed by Mohamed Shedeed

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "add.h"
#include "StudentInfo.h"
#include "groupinfo.h"
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
#include <QQueue>
#include <QPrinter>
#include <QTextDocument>
#include <QPrintDialog>
#include <QFontDatabase>
#include <QFont>
#include <QKeyEvent>
#include <QTabBar>

QSqlDatabase db;
bool first;
QNetworkReply *reply;
QQueue<int> stackMissed;
QQueue<int> stackAttended;
QQueue<QString> stackCompensation;

int dayCount;
int swappedCount;
bool swapped;
bool swapped2;
bool editingAttendance;
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

QString family;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    first = true;
    ui->setupUi(this);
    fillComboBoxes();


     ui->tabWidget->setTabText(0,"تسجيل");
     ui->tabWidget->setTabText(1,"حضور");

     editingAttendance = false;

    int id = QFontDatabase::addApplicationFont("./fonts/UniversNextArabic-Regular_2.ttf");
    family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font(family);

    DatabaseConnect(); // Connecting to database and organizing table

    DatabaseInit(); // Initializing database

    connect(ui->tableView_2, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));

    initializeAttendance();
}

void MainWindow::initializeAttendance()
{
    swapped = false;
    swapped2 = false;
    QFont font = ui->tableView_2->horizontalHeader()->font();
    font.setPointSize(12);
    ui->tableView_2->setFont(font);
    dayCount = 0;
    ui->dayComboBox1->addItem("");
    ui->dayComboBox1->addItem("الجمعة");
    ui->dayComboBox1->addItem("السبت");
    ui->dayComboBox1->addItem("الأحد");
    ui->dayComboBox1->addItem("الأثنين");
    ui->dayComboBox1->addItem("الثلاثاء");
    ui->dayComboBox1->addItem("الأربعاء");

    ui->dayComboBox2->addItem("");
    ui->dayComboBox2->addItem("الجمعة");
    ui->dayComboBox2->addItem("السبت");
    ui->dayComboBox2->addItem("الأحد");
    ui->dayComboBox2->addItem("الأثنين");
    ui->dayComboBox2->addItem("الثلاثاء");
    ui->dayComboBox2->addItem("الأربعاء");

    ui->groupComboBox1->addItem("");
    ui->groupComboBox1->addItem("مجموعة 1");
    ui->groupComboBox1->addItem("مجموعة 2");
    ui->groupComboBox1->addItem("مجموعة 3");
    ui->groupComboBox1->addItem("مجموعة 4");


    QLocale locale = QLocale(QLocale::Arabic, QLocale::Egypt);
    QDate date;
    date.setDate(2018,8,17);
    int dayCount = 0;
    swappedCount = 0;

    for (int i =0;i<40;i++)
    {
        friday[i] =  date.addDays(dayCount);
        saturday[i] =  date.addDays(dayCount+1);
        sunday[i] =  date.addDays(dayCount+2);
        monday[i] =  date.addDays(dayCount+3);
        tuesday[i] =  date.addDays(dayCount+4);
        wednesday[i] =  date.addDays(dayCount+5);
        dayCount+=7;
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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fillComboBoxes()
{
    ui->comboBox->addItem("");
    ui->comboBox->addItem("السبت");
    ui->comboBox->addItem("الأحد");
    ui->comboBox->addItem("الأثنين");
    ui->comboBox->addItem("الثلاثاء");
    ui->comboBox->addItem("الأربعاء");
    ui->comboBox->addItem("مؤقت");
    ui->comboBox->addItem("الارشيف");

    ui->comboBox_2->addItem("");
    ui->comboBox_2->addItem("مجموعة 1");
    ui->comboBox_2->addItem("مجموعة 2");
    ui->comboBox_2->addItem("مجموعة 3");
    ui->comboBox_2->addItem("مجموعة 4");

}

void MainWindow::on_pushButton_clicked() // Add
{
   Add add;
   connect(&add,SIGNAL(updatee()),this,SLOT(updateTbl()));
   add.setModal(true);
   add.exec();
}

bool MainWindow::eventFilter(QObject * obj, QEvent * ev){
    if(obj == ui->tableView)
    {
        if(ev->type() == QEvent::MouseButtonPress)
        {

        }
        else if(ev->type() == QEvent::KeyPress)
        {
            QKeyEvent *ke = static_cast<QKeyEvent*>(ev);
            if(ke->key() == Qt::Key_Delete)
               {
                QItemSelectionModel *select = ui->tableView->selectionModel();
                if ( select->currentIndex().column() == 0 ) {
                QString selection = ui->tableView->model()->data(select->currentIndex()).toString();

                QSqlQuery attendance;
                if (selection!="") {
                attendance.prepare("DELETE FROM student where id = ?");
                attendance.addBindValue(selection);
                attendance.exec(); } }

                on_searchBtn_clicked();
               }
            qDebug()<<"Key button pressed";
        }
    }
    return QObject::eventFilter(obj, ev);
}



void MainWindow::on_searchBtn_clicked() // Search
{
       QString s;
       s = ui->lineEdit->text();
       QSqlTableModel *model = new QSqlTableModel(ui->tableView,db);
       model->setTable("student");
       model->setEditStrategy(QSqlTableModel::OnFieldChange);
       model->select();
       model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
       model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
       model->setHeaderData(2, Qt::Horizontal, tr("مجموع الاعدادية"));
       model->setHeaderData(3, Qt::Horizontal, tr("رقم الهاتف"));
       model->setHeaderData(4, Qt::Horizontal, tr("رقم ولي الأمر"));
       model->setHeaderData(5, Qt::Horizontal, tr("المدرسة"));
       model->setHeaderData(6, Qt::Horizontal, tr("اليوم"));
       model->setHeaderData(7, Qt::Horizontal, tr("المجموعة"));
       model->setHeaderData(8, Qt::Horizontal, tr("منطقة السكن"));
       model->setHeaderData(9, Qt::Horizontal, tr("ملاحظات"));
       model->setHeaderData(10, Qt::Horizontal, tr("تاريخ التسجيل"));

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

        db.setDatabaseName("./student.db");

        if(!db.open())
            qWarning() << "MainWindow::DatabaseConnect - ERROR: " << db.lastError().text();
    }
    else
        qWarning() << "MainWindow::DatabaseConnect - ERROR: no driver " << DRIVER << " available";

    QSqlTableModel *model = new QSqlTableModel(ui->tableView,db);
    model->setTable("student");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
        model->select();
        model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
        model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
        model->setHeaderData(2, Qt::Horizontal, tr("مجموع الاعدادية"));
        model->setHeaderData(3, Qt::Horizontal, tr("رقم الهاتف"));
        model->setHeaderData(4, Qt::Horizontal, tr("رقم ولي الأمر"));
        model->setHeaderData(5, Qt::Horizontal, tr("المدرسة"));
        model->setHeaderData(6, Qt::Horizontal, tr("اليوم"));
        model->setHeaderData(7, Qt::Horizontal, tr("المجموعة"));
        model->setHeaderData(8, Qt::Horizontal, tr("منطقة السكن"));
        model->setHeaderData(9, Qt::Horizontal, tr("ملاحظات"));
        model->setHeaderData(10, Qt::Horizontal, tr("تاريخ التسجيل"));

    ui->tableView->setModel(model);

    for (int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c)
    {
        ui->tableView->horizontalHeader()->setSectionResizeMode(
            c, QHeaderView::Stretch);
    }

    QFont font = ui->tableView->horizontalHeader()->font();
    font.setPointSize(12);
    ui->tableView->setFont(font);

    ui->tableView->horizontalHeader()->swapSections(0,55);
    ui->tableView->horizontalHeader()->swapSections(1,54);
    ui->tableView->horizontalHeader()->swapSections(2,53);
    ui->tableView->horizontalHeader()->swapSections(3,52);
    ui->tableView->horizontalHeader()->swapSections(4,51);
    ui->tableView->horizontalHeader()->swapSections(5,50);
    ui->tableView->horizontalHeader()->swapSections(6,49);
    ui->tableView->horizontalHeader()->swapSections(7,48);
    ui->tableView->horizontalHeader()->swapSections(8,47);
    ui->tableView->horizontalHeader()->swapSections(9,46);
    ui->tableView->horizontalHeader()->swapSections(10,45);
       for (int i=11;i<57;i++)
       ui->tableView->hideColumn(i);

    ui->tableView->resizeColumnsToContents();
    //ui->tableView->verticalHeader()->setVisible(false);

    ui->tableView->installEventFilter(this);
    ui->tableView->show();
}

void MainWindow::updateTbl()
{
    QSqlTableModel *model = new QSqlTableModel(ui->tableView,db);
       model->setTable("student");
       model->setEditStrategy(QSqlTableModel::OnFieldChange);
       model->select();
       model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
       model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
       model->setHeaderData(2, Qt::Horizontal, tr("مجموع الاعدادية"));
       model->setHeaderData(3, Qt::Horizontal, tr("رقم الهاتف"));
       model->setHeaderData(4, Qt::Horizontal, tr("رقم ولي الأمر"));
       model->setHeaderData(5, Qt::Horizontal, tr("المدرسة"));
       model->setHeaderData(6, Qt::Horizontal, tr("اليوم"));
       model->setHeaderData(7, Qt::Horizontal, tr("المجموعة"));
       model->setHeaderData(8, Qt::Horizontal, tr("منطقة السكن"));
       model->setHeaderData(9, Qt::Horizontal, tr("ملاحظات"));
       model->setHeaderData(10, Qt::Horizontal, tr("تاريخ التسجيل"));

    while(model->canFetchMore()) {model->fetchMore();}

   ui->tableView->setModel(model);

   QFont font = ui->tableView->horizontalHeader()->font();
   font.setPointSize(12);
   ui->tableView->setFont(font);

   if (first)
   {
       ui->tableView->horizontalHeader()->swapSections(0,55);
       ui->tableView->horizontalHeader()->swapSections(1,54);
       ui->tableView->horizontalHeader()->swapSections(2,53);
       ui->tableView->horizontalHeader()->swapSections(3,52);
       ui->tableView->horizontalHeader()->swapSections(4,51);
       ui->tableView->horizontalHeader()->swapSections(5,50);
       ui->tableView->horizontalHeader()->swapSections(6,49);
       ui->tableView->horizontalHeader()->swapSections(7,48);
       ui->tableView->horizontalHeader()->swapSections(8,47);
       ui->tableView->horizontalHeader()->swapSections(9,46);
       ui->tableView->horizontalHeader()->swapSections(10,45);
       first = false;
   }

   for (int i=11;i<57;i++)
      ui->tableView->hideColumn(i);

   //ui->tableView->verticalHeader()->setVisible(false);
   ui->tableView->show();
   ui->tableView->scrollToBottom();
}

void MainWindow::DatabaseInit()
{
    if (db.tables().contains(QLatin1String("student"))) {first = false;}
    else { first = true;
    QSqlQuery query("CREATE TABLE student(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, grade TEXT, number TEXT, parent TEXT, school TEXT, day TEXT, time TEXT, address TEXT, notes TEXT, regdate TEXT, exam1 TEXT, exam2 TEXT, exam3 TEXT, exam4 TEXT, exam5 TEXT, exam6 TEXT, day1 TEXT, day2 TEXT, day3 TEXT, day4 TEXT, day5 TEXT, day6 TEXT, day7 TEXT, day8 TEXT, day9 TEXT, day10 TEXT, day11 TEXT, day12 TEXT, day13 TEXT, day14 TEXT, day15 TEXT, day16 TEXT, day17 TEXT, day18 TEXT, day19 TEXT, day20 TEXT, day21 TEXT, day22 TEXT, day23 TEXT, day24 TEXT, day25 TEXT, day26 TEXT, day27 TEXT, day28 TEXT, day29 TEXT, day30 TEXT, day31 TEXT, day32 TEXT, day33 TEXT, day34 TEXT, day35 TEXT, day36 TEXT, day37 TEXT, day38 TEXT, day39 TEXT, day40 TEXT, CONSTRAINT name_unique UNIQUE (name))");

    if(!query.isActive())
        qWarning() << "MainWindow::DatabaseInit - ERROR: " << query.lastError().text(); }
}


void MainWindow::on_refreshBtn_clicked() // Reset
{
    updateTbl();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
}


void MainWindow::upload()
{
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-sqlite3"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"db\"; filename=\"1.db\""));
    QFile *file = new QFile("./student.db");
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
//    switch (index)
//    {
//        case 1:
//    {
//        ui->comboBox_2->clear();
//        ui->comboBox_2->addItem("09:00");
//        ui->comboBox_2->addItem("04:00");
//        ui->comboBox_2->addItem("06:00");
//        break;
//    }
//    default:
//        ui->comboBox_2->clear();
//        ui->comboBox_2->addItem("");
//        ui->comboBox_2->addItem("09:00");
//        ui->comboBox_2->addItem("11:00");
//        ui->comboBox_2->addItem("01:00");
//        ui->comboBox_2->addItem("03:00");
//        ui->comboBox_2->addItem("05:00");
//        ui->comboBox_2->addItem("07:00");
//        break;
//    }
}

void MainWindow::on_pushButton_6_clicked()
{
    upload();
}


void MainWindow::on_pushButton_7_clicked()
{
    QTableView *tableView = ui->tableView;
    handleReport(tableView);
}

void MainWindow::handleReport(QTableView *tableView)
{
    int count = 0;
    QItemSelectionModel *select = tableView->selectionModel();
    if ( select->currentIndex().column() == 0 ) {
    QString selection = tableView->model()->data(select->currentIndex()).toString();

    QSqlQuery attendance;
    attendance.prepare("SELECT * FROM student where id = ?");

    attendance.addBindValue(selection);
    attendance.exec();
    attendance.first();

    QString day = attendance.value(6).toString();

    for (int i =17;i<57;i++)
    {
        if (attendance.value(i).toString() == "غائب") { stackMissed.enqueue(i);
            count++; }
        else if (QString::compare(attendance.value(i).toString(), "حاضر", Qt::CaseInsensitive) == 0)
        {
            stackAttended.enqueue(i);
        }
        else if (attendance.value(i).toString().contains("-"))
        {
            stackCompensation.enqueue(attendance.value(i).toString());
        }
    }


    StudentInfo stdnfo;
    stdnfo.setModal(true);
    QString test = select->currentIndex().sibling(select->currentIndex().row(),1).data().toString();
    if (day=="السبت")
        stdnfo.setData(test,QString::number(count),stackMissed,stackAttended,stackCompensation,saturday,family);
    else if (day=="الأحد")
        stdnfo.setData(test,QString::number(count),stackMissed,stackAttended,stackCompensation,sunday,family);
    else if (day=="الأثنين")
        stdnfo.setData(test,QString::number(count),stackMissed,stackAttended,stackCompensation,monday,family);
    else if (day=="الثلاثاء")
        stdnfo.setData(test,QString::number(count),stackMissed,stackAttended,stackCompensation,tuesday,family);
    else if (day=="الأربعاء")
        stdnfo.setData(test,QString::number(count),stackMissed,stackAttended,stackCompensation,wednesday,family);



    stdnfo.exec(); }
}


void MainWindow::on_pushButton_9_clicked()
{
        QString strStream;
        QTextStream out(&strStream);

        const int rowCount = ui->tableView->model()->rowCount();
        const int columnCount = ui->tableView->model()->columnCount();

        out <<  "<html>\n"
            "<head>\n"
            "<meta Content=\"Text/html; charset=Windows-1251\">\n"
            <<  QString("<title>%1</title>\n").arg("shedeed.html")
            <<  "</head>\n"
            "<body bgcolor=#ffffff link=#5000A0>\n"
            "<table border=1 cellspacing=0 cellpadding=2>\n";

        // headers
        out << "<thead><tr bgcolor=#f0f0f0>";
        for (int column = columnCount - 1; column >= 0; column--)
            if (!ui->tableView->isColumnHidden(column))
                out << QString("<th>%1</th>").arg(ui->tableView->model()->headerData(column, Qt::Horizontal).toString());
        out << "</tr></thead>\n";

        // data table
        for (int row = 0; row < rowCount; row++) {
            out << "<tr>";
            for (int column = columnCount - 1; column >= 0; column--) {
                if (!ui->tableView->isColumnHidden(column)) {
                    QString data = ui->tableView->model()->data(ui->tableView->model()->index(row, column)).toString().simplified();
                    out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                }
            }
            out << "</tr>\n";
        }
        out <<  "</table>\n"
            "</body>\n"
            "</html>\n";

        QTextDocument *document = new QTextDocument();
        document->setHtml(strStream);

        QPrinter printer;

        QPrintDialog *dialog = new QPrintDialog(&printer, NULL);
        if (dialog->exec() == QDialog::Accepted) {
            document->print(&printer);
        }

        delete document;
}

void MainWindow::on_viewAttendance_clicked()
{
    editingAttendance = true;
    QString s = ui->dayComboBox2->currentText();
    QString s1 = QString::number(ui->dateComboBox->currentIndex());
    QString s2 = ui->groupComboBox2->currentText();
    int data = ui->dateComboBox->currentIndex();

    QSqlDatabase db;
    QSqlTableModel *model = new QSqlTableModel(ui->tableView_2,db);

    if (data!= -1 || s != "") {

     for (int i=0;i<57;i++)
       ui->tableView_2->showColumn(i);

    ui->result_label->clear();

    qDebug() << s1;

    model->setTable("student");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);

    model->select();

    model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
    model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
    model->setHeaderData(17+data, Qt::Horizontal, tr("حــضــور"));

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

   ui->tableView_2->setModel(model);
   if (swappedCount == 0)
   {
       if (!swapped2) {
   ui->tableView_2->horizontalHeader()->swapSections(0,55);
   ui->tableView_2->horizontalHeader()->swapSections(1,54); }
   }
   swapped = true;
   swappedCount = 1;

   for (int i=2;i<57;i++)
    if (i!=(17+ui->dateComboBox->currentIndex()))
        ui->tableView_2->hideColumn(i);
   for (int c = 0; c < ui->tableView_2->horizontalHeader()->count(); ++c)
   {
       ui->tableView_2->horizontalHeader()->setSectionResizeMode(
           c, QHeaderView::Stretch);
   }
   //ui->tableView_2->verticalHeader()->setVisible(false);
   ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
   ui->tableView_2->show();
   }

    else

    {
        ui->result_label->setText("ادخل اليوم لعرض وتعديل الحضور"); }
}

void MainWindow::on_dayComboBox2_currentIndexChanged(int index)
{
    ui->groupComboBox2->clear();
    ui->groupComboBox2->addItem("");
    ui->groupComboBox2->addItem("مجموعة 1");
    ui->groupComboBox2->addItem("مجموعة 2");
    ui->groupComboBox2->addItem("مجموعة 3");
    ui->groupComboBox2->addItem("مجموعة 4");
    ui->groupComboBox2->addItem("مجموعة 5");
    switch (index)
    {
    case 0:
        ui->dateComboBox->clear();
        break;
    case 1:
        ui->dateComboBox->clear();
        for (int i = 0;i<40;i++)
        ui->dateComboBox->addItem(QString::number(i+1) + "- " + Sfriday[i]);
        break;
    case 2:
        ui->dateComboBox->clear();
        for (int i = 0;i<40;i++)
        ui->dateComboBox->addItem(QString::number(i+1) + "- " + Ssaturday[i]);
        break;
    case 3:
        ui->dateComboBox->clear();
        for (int i = 0;i<40;i++)
        ui->dateComboBox->addItem(QString::number(i+1) + "- " + Ssunday[i]);
        break;
    case 4:
        ui->dateComboBox->clear();
        for (int i = 0;i<40;i++)
        ui->dateComboBox->addItem(QString::number(i+1) + "- " + Smonday[i]);
        break;
    case 5:
        ui->dateComboBox->clear();
        for (int i = 0;i<40;i++)
        ui->dateComboBox->addItem(QString::number(i+1)+ "- " + Stuesday[i]);
        break;
    case 6:
        ui->dateComboBox->clear();
        for (int i = 0;i<40;i++)
        ui->dateComboBox->addItem(QString::number(i+1) + "- " + Swednesday[i]);
        break;
    }
}

void MainWindow::on_viewGradesBtn_clicked()
{
    editingAttendance = false;
    for (int i=0;i<57;i++)
      ui->tableView_2->showColumn(i);
    QSqlDatabase db;
    QString s = ui->name1->text();
    QSqlTableModel *model = new QSqlTableModel(ui->tableView_2,db);
    model->setTable("student");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);

    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("الرقم التعريفي"));
    model->setHeaderData(1, Qt::Horizontal, tr("الإسم"));
    model->setHeaderData(11, Qt::Horizontal, tr("امتحان 1"));
    model->setHeaderData(12, Qt::Horizontal, tr("امتحان 2"));
    model->setHeaderData(13, Qt::Horizontal, tr("امتحان 3"));
    model->setHeaderData(14, Qt::Horizontal, tr("امتحان 4"));
    model->setHeaderData(15, Qt::Horizontal, tr("امتحان 5"));
    model->setHeaderData(16, Qt::Horizontal, tr("امتحان 6"));


   if (ui->dayComboBox1->currentText()=="" && ui->groupComboBox1->currentText()=="") {
   if (ui->id1->text() == "" && ui->name1->text() != "")
   model->setFilter(QString("name like '%%1%'").arg(s));
   else if (ui->name1->text() == "" && ui->id1->text() != "")
    model->setFilter(QString("id like '%1%'").arg(ui->id1->text()));
   else if (ui->name1->text() != "" && ui->id1->text() != "")
       model->setFilter(QString("name like '%%1%' AND id like '%2%'").arg(s).arg(ui->id1->text() != ""));
   else if (ui->name1->text() == "" && ui->id1->text() == "")
   {} }

   else if (ui->dayComboBox1->currentText()!="" && ui->groupComboBox1->currentText()=="")
   {
       if (ui->id1->text() == "" && ui->name1->text() != "")
       model->setFilter(QString("name like '%%1%' AND day like '%%2%'").arg(s).arg(ui->dayComboBox1->currentText()));
       else if (ui->name1->text() == "" && ui->id1->text() != "")
        model->setFilter(QString("id like '%1%' AND day like '%%2%'").arg(ui->id1->text()).arg(ui->dayComboBox1->currentText()));
       else if (ui->name1->text() != "" && ui->id1->text() != "")
           model->setFilter(QString("name like '%%1%' AND id like '%2%' AND day like '%%3%'").arg(s).arg(ui->id1->text()).arg(ui->dayComboBox1->currentText()));
       else if (ui->name1->text() == "" && ui->id1->text() == "")
       {model->setFilter(QString("day like '%%1%'").arg(ui->dayComboBox1->currentText()));}
   }

   else if (ui->dayComboBox1->currentText()=="" && ui->groupComboBox1->currentText()!="")
   {
       if (ui->id1->text() == "" && ui->name1->text() != "")
       model->setFilter(QString("name like '%%1%' AND time like '%%2%'").arg(s).arg(ui->groupComboBox1->currentText()));
       else if (ui->name1->text() == "" && ui->id1->text() != "")
        model->setFilter(QString("id like '%1%' AND time like '%%2%'").arg(ui->id1->text()).arg(ui->groupComboBox1->currentText()));
       else if (ui->name1->text() != "" && ui->id1->text() != "")
           model->setFilter(QString("name like '%%1%' AND id like '%2%' AND time like '%%3%'").arg(s).arg(ui->id1->text()).arg(ui->groupComboBox1->currentText()));
       else if (ui->name1->text() == "" && ui->id1->text() == "")
       {model->setFilter(QString("time like '%%1%'").arg(ui->groupComboBox1->currentText()));}
   }

   else if (ui->dayComboBox1->currentText()!="" && ui->groupComboBox1->currentText()!="")
   {
       if (ui->id1->text() == "" && ui->name1->text() != "")
       model->setFilter(QString("name like '%%1%' AND day like '%%2' AND time like '%%3%'").arg(s).arg(ui->dayComboBox1->currentText()).arg(ui->groupComboBox1->currentText()));
       else if (ui->name1->text() == "" && ui->id1->text() != "")
        model->setFilter(QString("id like '%1%' AND day like '%%2%' AND time like '%%3'").arg(ui->id1->text()).arg(ui->dayComboBox1->currentText()).arg(ui->groupComboBox1->currentText()));
       else if (ui->name1->text() != "" && ui->id1->text() != "")
           model->setFilter(QString("name like '%%1%' AND id like '%2%' AND day like '%%3%' AND time like '%%4%'").arg(s).arg(ui->id1->text()).arg(ui->dayComboBox1->currentText()).arg(ui->groupComboBox1->currentText()));
       else if (ui->name1->text() == "" && ui->id1->text() == "")
       {model->setFilter(QString("day like '%%1%' AND time like '%%2%'").arg(ui->dayComboBox1->currentText()).arg(ui->groupComboBox1->currentText()));}
   }

   ui->tableView_2->setModel(model);

   QFont font = ui->tableView_2->horizontalHeader()->font();
   font.setPointSize(12);
   ui->tableView_2->setFont(font);

   ui->tableView_2->resizeColumnsToContents();
   //ui->tableView_2->resizeRowsToContents();
   //ui->tableView_2->verticalHeader()->setVisible(false);

   for (int i=2;i<11;i++)
   ui->tableView_2->hideColumn(i);

   for (int i=17;i<57;i++)
   ui->tableView_2->hideColumn(i);
   ui->tableView_2->setEditTriggers(QAbstractItemView::AnyKeyPressed);
   //ui->tableView_2->setEditTriggers(QAbstractItemView::DoubleClicked);

   ui->tableView_2->show();

   if ( dayCount == 0 )
   {
       if (!swapped) {
      ui->tableView_2->horizontalHeader()->swapSections(0,55);
      ui->tableView_2->horizontalHeader()->swapSections(1,54); }
      ui->tableView_2->horizontalHeader()->swapSections(11,53);
      ui->tableView_2->horizontalHeader()->swapSections(12,52);
      ui->tableView_2->horizontalHeader()->swapSections(13,51);
      ui->tableView_2->horizontalHeader()->swapSections(14,50);
      ui->tableView_2->horizontalHeader()->swapSections(15,49);
      ui->tableView_2->horizontalHeader()->swapSections(16,48);

   }
   swapped2 = true;


   for (int c = 0; c < ui->tableView_2->horizontalHeader()->count(); ++c)
   {
       ui->tableView_2->horizontalHeader()->setSectionResizeMode(
           c, QHeaderView::Stretch);
   }
   dayCount = 1;
}

void MainWindow::onTableClicked(const QModelIndex &index)
{
    if (editingAttendance)
    {
    if (index.isValid()) {
        QString cellText = index.data().toString();
        if (cellText == "" || cellText=="غائب")
        {
            QSqlQuery q;
            q.prepare("UPDATE student SET day" + QString::number(index.column()-16) + "= ? WHERE id = ?");
            q.addBindValue("حاضر");
            q.addBindValue(index.sibling(index.row(),0).data().toString());

            if(!q.exec())
                qWarning() << "Add - ERROR: " << q.lastError().text();
            else
            {
                on_viewAttendance_clicked();
            }
        }
        else
        {
            QSqlQuery q;
            q.prepare("UPDATE student SET day" + QString::number(index.column()-16) + "= ? WHERE id = ?");
            q.addBindValue("غائب");
            q.addBindValue(index.sibling(index.row(),0).data().toString());

            if(!q.exec())
                qWarning() << "Add - ERROR: " << q.lastError().text();
            else
            {
                on_viewAttendance_clicked();
            }
        }
    }
    }

}

void MainWindow::on_compensationBtn_clicked()
{
    QString idCompensation = ui->idCompensation->text();

    if (ui->dateComboBox->currentText()!="" && ui->groupComboBox2->currentText()!="")
    {
        ui->result_label->clear();
        QTime time;
        QSqlQuery q;
        //qDebug() << "UPDATE student SET day" + QString::number(ui->dateComboBox->currentIndex()) + "= ? WHERE id = ?";
        q.prepare("UPDATE student SET day" + QString::number(ui->dateComboBox->currentIndex()+1) + "= ? WHERE id = ?");
        q.addBindValue("تعويض - " + ui->dateComboBox->currentText() + "- " + ui->groupComboBox2->currentText() + "- " + time.currentTime().toString() );
        q.addBindValue(idCompensation);

        if(!q.exec())
            qWarning() << "Add - ERROR: " << q.lastError().text();
        else
        {
            on_viewAttendance_clicked();
        }
    }
    else
    {
        ui->result_label->setText("الرجاء ادخال جميع الخانات");
    }
}

void MainWindow::on_recordAttendanceToAll_clicked()
{
    if (ui->dayComboBox2->currentText()!="" && ui->dateComboBox->currentText()!="" && ui->groupComboBox2->currentText()!="")
    {
        ui->result_label->clear();
        QSqlQuery q;
        q.prepare("UPDATE student SET day" + QString::number(ui->dateComboBox->currentIndex()+1) + "= ? WHERE day = ? AND time = ?");
        q.addBindValue("حاضر");
        q.addBindValue(ui->dayComboBox2->currentText());
        q.addBindValue(ui->groupComboBox2->currentText());

        if(!q.exec())
            qWarning() << "Add - ERROR: " << q.lastError().text();
        else
            on_viewAttendance_clicked();
    }
    else
    {
        ui->result_label->setText("الرجاء ادخال جميع الخانات");
    }
}

void MainWindow::on_showGroupReport_clicked()
{
    if (ui->dayComboBox2->currentText()!="" && ui->dateComboBox->currentText()!="" && ui->groupComboBox2->currentText()!="")
    {
        ui->result_label->clear();
        QSqlQuery attendance;
        attendance.prepare("SELECT COUNT(id) FROM student WHERE day = ? AND time = ? AND day" + QString::number(ui->dateComboBox->currentIndex()+1) + "= ?");
        attendance.addBindValue(ui->dayComboBox2->currentText());
        attendance.addBindValue(ui->groupComboBox2->currentText());
        attendance.addBindValue("حاضر");
        attendance.exec();
        attendance.first();

        QSqlQuery missed;
        missed.prepare("SELECT COUNT(id) FROM student WHERE day = ? AND time = ? AND day" + QString::number(ui->dateComboBox->currentIndex()+1) + "= ?");
        missed.addBindValue(ui->dayComboBox2->currentText());
        missed.addBindValue(ui->groupComboBox2->currentText());
        missed.addBindValue("غائب");
        missed.exec();
        missed.first();

        QSqlQuery comp;
        comp.prepare("SELECT COUNT(id) FROM student WHERE day = ? AND time = ? AND day" + QString::number(ui->dateComboBox->currentIndex()+1) + " IS NOT NULL AND day" + QString::number(ui->dateComboBox->currentIndex()+1) + "!= ? AND day" + QString::number(ui->dateComboBox->currentIndex()+1) + "!= ?");
        comp.addBindValue(ui->dayComboBox2->currentText());
        comp.addBindValue(ui->groupComboBox2->currentText());
        comp.addBindValue("حاضر");
        comp.addBindValue("غائب");
        comp.exec();
        comp.first();



        groupinfo grpinfo;
        grpinfo.setModal(true);
        grpinfo.setData(ui->groupComboBox2->currentText(),ui->dayComboBox2->currentText(),ui->dateComboBox->currentText(),attendance.value(0).toString(),missed.value(0).toString(),comp.value(0).toString());
        grpinfo.exec();
    }
    else
    {
        ui->result_label->setText("الرجاء ادخال جميع الخانات");
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    QTableView *tableView = ui->tableView_2;
    handleReport(tableView);
}
