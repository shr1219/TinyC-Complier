#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lexicalanalysis.h"
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QByteArray>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("语义分析及中间代码");
    this->setFixedSize(1080,600);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_actionopen_triggered()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("文件选择");
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    fileDialog->setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (fileDialog->exec()) {
       fileNames = fileDialog->selectedFiles();
    }
    if(fileNames.isEmpty())
        return;
    cur_path=fileNames.front();
    if(!cur_path.isEmpty()){
        QFile file(cur_path);
        if(file.open(QIODevice::ReadOnly)){
            QByteArray text=file.readAll();
            ui->textEdit->setText(text);
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    My_ECode=0;
    My_EMess="";
    ParseAnalysis ppa;
    std::string path;
    path =cur_path.toLocal8Bit().constData();
    std::cout<<path<<std::endl;
    std::ifstream ifs(path);
    LexicalAnalysis la(ifs);
    std::string str=ppa.do_analyze(la);
    if(My_ECode==1)
        str="(ErrorCode:1)Lexical Error exists!!";
    if(My_ECode==2)
        str="(ErrorCode:2)Parse Error exists!!";
    if(My_ECode==3)
        str="(ErrorCode:3)Semantic Error exists!!";
    if(My_ECode){
        str+="\n";
        str+=My_EMess;
    }
    QString qs = QString::fromLocal8Bit(QByteArray::fromRawData(str.c_str(), str.size()));
    ui->textBrowser_1->setText(qs);
}

void MainWindow::on_pushButton_2_clicked()
{
   QFile file(cur_path);
   if(file.open(QIODevice::WriteOnly)){
       QString str = ui->textEdit->toPlainText();
       file.write(str.toStdString().data());
   }
}

void MainWindow::on_pushButton_3_clicked()
{
    My_ECode=0;
    My_EMess="";
    ParseAnalysis ppa;
    std::string path;
    path =cur_path.toLocal8Bit().constData();
    std::cout<<path<<std::endl;
    std::ifstream ifs(path);
    LexicalAnalysis la(ifs);
    ppa.do_analyze(la);
    std::string str=ppa.get_semantic();
    if(My_ECode==1)
        str="(ErrorCode:1)Lexical Error exists!!";
    if(My_ECode==2)
        str="(ErrorCode:2)Parse Error exists!!";
    if(My_ECode==3)
        str="(ErrorCode:3)Semantic Error exists!!";
    if(My_ECode){
        str+="\n";
        str+=My_EMess;
    }
    QString qs = QString::fromLocal8Bit(QByteArray::fromRawData(str.c_str(), str.size()));
    ui->textBrowser_1->setText(qs);
}


void MainWindow::on_actionparse_triggered()
{
    ui->textBrowser_1->clear();
    std::string str=pa.init_ret;
    QString qs = QString::fromLocal8Bit(QByteArray::fromRawData(str.c_str(), str.size()));
    ui->textBrowser_1->setText(qs);
}

void MainWindow::on_actionfirst_triggered()
{
    ui->textBrowser_1->clear();
    std::string str=pa.first_ret;
    QString qs = QString::fromLocal8Bit(QByteArray::fromRawData(str.c_str(), str.size()));
    ui->textBrowser_1->setText(qs);
}

void MainWindow::on_actionfollow_triggered()
{
    ui->textBrowser_1->clear();
    std::string str=pa.follow_ret;
    QString qs = QString::fromLocal8Bit(QByteArray::fromRawData(str.c_str(), str.size()));
    ui->textBrowser_1->setText(qs);
}

void MainWindow::on_actiontable_triggered()
{
    ui->textBrowser_1->clear();
    std::string str=pa.table_ret;
    QString qs = QString::fromLocal8Bit(QByteArray::fromRawData(str.c_str(), str.size()));
    ui->textBrowser_1->setText(qs);
}

