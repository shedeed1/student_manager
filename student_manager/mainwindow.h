#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void DatabaseConnect();
    void initializeAttendance();
    void fillComboBoxes();
    void DatabaseInit();
    void on_pushButton_clicked();
    void on_searchBtn_clicked();
    void updateTbl();
    void on_refreshBtn_clicked();
    void upload();
    void on_comboBox_currentIndexChanged(int index);
    void uploadProgress(qint64, qint64 );
    void uploadDone();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();


    void on_pushButton_9_clicked();


    bool eventFilter(QObject *,QEvent*);

    void on_viewAttendance_clicked();

    void on_dayComboBox2_currentIndexChanged(int index);

    void on_viewGradesBtn_clicked();
    void onTableClicked(const QModelIndex &);

    void on_compensationBtn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
