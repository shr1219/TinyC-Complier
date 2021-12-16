#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "parseanalysis.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionopen_triggered();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_actionparse_triggered();

    void on_actionfirst_triggered();

    void on_actionfollow_triggered();

    void on_actiontable_triggered();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QString cur_path;
    ParseAnalysis pa;
};
#endif // MAINWINDOW_H
