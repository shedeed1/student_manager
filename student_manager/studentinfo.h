#ifndef STUDENTINFO_H
#define STUDENTINFO_H

#include <QDialog>
#include <QQueue>
#include <QDate>

namespace Ui {
class StudentInfo;
}

class StudentInfo : public QDialog
{
    Q_OBJECT

public:
    explicit StudentInfo(QWidget *parent = 0);
    void setData(const QString &labelText,const QString &labelText2,QQueue<int> &,QQueue<int>&,QQueue<QString>&,QDate[],QString);
    ~StudentInfo();

private:
    Ui::StudentInfo *ui;
};

#endif // STUDENTINFO_H
