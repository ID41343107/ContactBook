#include "mywidget.h"
#include "ui_mywidget.h"

#include <QFile>
#include <QDebug>

QString mFilename = "C:user\Desktop\1110\ContactBook";

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

