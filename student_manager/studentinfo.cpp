#include "studentinfo.h"
#include "ui_studentinfo.h"

#include <QStack>
#include <QDebug>
#include <QDate>
#include <QFontDatabase>
#include <QFont>

StudentInfo::StudentInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentInfo)
{
    ui->setupUi(this);
    int id = QFontDatabase::addApplicationFont("./fonts/UniversNextArabic-Regular_2.ttf");


}

void StudentInfo::setData(const QString &labelText, const QString &labelText2, QStack<int> &missedStack,QStack<int> &attendedStack,QStack<QString> &compensationStack, QDate dates[],QString family) {
  ui->label_3->setText(labelText);
  ui->label_4->setText(labelText2);
  QString arabicdate;
  QLocale locale = QLocale(QLocale::Arabic, QLocale::Egypt);
  while (!missedStack.isEmpty())
  {
      arabicdate = locale.toString(dates[missedStack.pop() - 17]);
      ui->listWidget->addItem(arabicdate);
  }
  while(!attendedStack.isEmpty())
  {
      arabicdate = locale.toString(dates[attendedStack.pop() - 17]);
      ui->listWidget_2->addItem(arabicdate);
  }
  while(!compensationStack.isEmpty())
  {
      ui->listWidget_2->addItem(compensationStack.pop());
  }

  QFont fontUnderlined(family);
  QFont font(family);
  fontUnderlined.setUnderline(true);
  fontUnderlined.setPointSize(16);

  font.setPointSize(12);
  font.setBold(true);

  ui->label->setFont(fontUnderlined);
  ui->label_2->setFont(fontUnderlined);
  ui->label_3->setFont(font);
  ui->label_4->setFont(font);

}

StudentInfo::~StudentInfo()
{
    delete ui;
}
