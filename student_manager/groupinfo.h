#ifndef GROUPINFO_H
#define GROUPINFO_H

#include <QDialog>

namespace Ui {
class groupinfo;
}

class groupinfo : public QDialog
{
    Q_OBJECT

public:
    explicit groupinfo(QWidget *parent = 0);
    void setData(const QString &,const QString &,const QString &,const QString &,const QString &, const QString&);
    ~groupinfo();

private:
    Ui::groupinfo *ui;
};

#endif // GROUPINFO_H
