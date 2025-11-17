#include "mywidget.h"
#include "ui_mywidget.h"

#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>

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
            saveFile+=ui->tableWidget->item(i,j)->text()+",";
        }
        saveFile+="\n";
    }
    Write (mFilename, saveFile); //呼叫Write
}

void MyWidget::on_pushButton_3_clicked()
{
    // 匯入按鈕 - Import data from file
    QString filename = QFileDialog::getOpenFileName(this, 
        QStringLiteral("選擇檔案"), 
        "", 
        "Text Files (*.txt);;CSV Files (*.csv);;All Files (*)");
    
    if (filename.isEmpty()) {
        return; // User cancelled
    }
    
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QStringLiteral("錯誤"), QStringLiteral("無法開啟檔案"));
        return;
    }
    
    // Clear existing data
    ui->tableWidget->setRowCount(0);
    
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        
        if (fields.size() >= 4) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            
            for (int i = 0; i < 4 && i < fields.size(); i++) {
                QTableWidgetItem *item = new QTableWidgetItem(fields[i].trimmed());
                ui->tableWidget->setItem(row, i, item);
            }
        }
    }
    
    file.close();
}

void MyWidget::on_pushButton_4_clicked()
{
    // 結束按鈕 - Save data and exit
    // First save the data
    QString saveFile="";
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            if (ui->tableWidget->item(i, j)) {
                saveFile += ui->tableWidget->item(i, j)->text() + ",";
            } else {
                saveFile += ",";
            }
        }
        saveFile += "\n";
    }
    Write(mFilename, saveFile);
    
    // Then close the application
    QApplication::quit();
}

