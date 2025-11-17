#include "mywidget.h"
#include "ui_mywidget.h"

#include <QFile>
#include <QDebug>

QString mFilename = "C:/Users/user/Desktop/1117/myfile.txt";

void Write (QString Filename, QString str)
{
    QFile mFile (Filename);
    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        qDebug() <<"could not open file for write";
        return;
    }
    QTextStream out (&mFile);
    out<<str; //將內容輸出至txt檔
    mFile.flush(); //刷新mFile
    mFile.close(); //關閉mFile
}

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    QStringList ColTotle;

    ui->tableWidget->setColumnCount (4);
    ColTotle<<QStringLiteral ("學號")<<QStringLiteral ("班級")<<QStringLiteral ("姓名")<<QStringLiteral ("電話");
    ui->tableWidget->setHorizontalHeaderLabels (ColTotle);
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_pushButton_clicked()
{
    QTableWidgetItem *inputRow1,*inputRow2,*inputRow3,*inputRow4;

    inputRow1= new QTableWidgetItem(QString (ui->lineEdit->text()));
    inputRow2= new QTableWidgetItem(QString(ui->lineEdit_2->text()));
    inputRow3= new QTableWidgetItem(QString (ui->lineEdit_3->text()));
    inputRow4= new QTableWidgetItem(QString (ui->lineEdit_4->text()));

    ui->tableWidget->insertRow (ui->tableWidget->rowCount());

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,inputRow1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1,inputRow2);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,inputRow3);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,inputRow4);
}


void MyWidget::on_pushButton_2_clicked()
{
    QString saveFile=""; //存檔內容
    for(int i = 0 ;i<ui->tableWidget->rowCount();i++) {
        for (int j=0;j<ui->tableWidget->columnCount();j++) {
            saveFile+=ui->tableWidget->item(1,3)->text()+",";
        }
        saveFile+="\n";
    }
    Write (mFilename, saveFile); //呼叫Write
}

