#include "mywidget.h"
#include "ui_mywidget.h"

#include <QFile>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>

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
    // Validate input - ensure all fields are filled
    QString studentId = ui->lineEdit->text().trimmed();
    QString className = ui->lineEdit_2->text().trimmed();
    QString name = ui->lineEdit_3->text().trimmed();
    QString phone = ui->lineEdit_4->text().trimmed();
    
    if (studentId.isEmpty() || className.isEmpty() || name.isEmpty() || phone.isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("輸入錯誤"), 
                           QStringLiteral("請填寫所有欄位！"));
        return;
    }
    
    QTableWidgetItem *inputRow1,*inputRow2,*inputRow3,*inputRow4;

    inputRow1= new QTableWidgetItem(studentId);
    inputRow2= new QTableWidgetItem(className);
    inputRow3= new QTableWidgetItem(name);
    inputRow4= new QTableWidgetItem(phone);

    ui->tableWidget->insertRow (ui->tableWidget->rowCount());

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,inputRow1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1,inputRow2);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,inputRow3);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,inputRow4);
    
    // Clear input fields after successful addition
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    
    // Focus back to first field for convenience
    ui->lineEdit->setFocus();
    
    // Show success message
    QMessageBox::information(this, QStringLiteral("成功"), 
                            QStringLiteral("聯絡人已成功新增！"));
}


void MyWidget::on_pushButton_2_clicked()
{
    // Check if there's data to export
    if (ui->tableWidget->rowCount() == 0) {
        QMessageBox::warning(this, QStringLiteral("匯出錯誤"), 
                           QStringLiteral("沒有資料可以匯出！"));
        return;
    }
    
    // Let user choose where to save the file
    QString filename = QFileDialog::getSaveFileName(this, 
        QStringLiteral("儲存檔案"), 
        QDir::homePath() + "/contacts.txt",
        "Text Files (*.txt);;CSV Files (*.csv);;All Files (*)");
    
    if (filename.isEmpty()) {
        return; // User cancelled
    }
    
    QString saveFile=""; //存檔內容
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        for (int j = 0; j < ui->tableWidget->columnCount(); j++) {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            if (item) {
                saveFile += item->text() + ",";
            } else {
                saveFile += ",";
            }
        }
        saveFile += "\n";
    }
    
    // Save to the chosen file
    QFile mFile(filename);
    if(!mFile.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::critical(this, QStringLiteral("錯誤"), 
                            QStringLiteral("無法開啟檔案進行寫入！"));
        return;
    }
    QTextStream out(&mFile);
    out << saveFile;
    mFile.flush();
    mFile.close();
    
    // Show success message
    QMessageBox::information(this, QStringLiteral("成功"), 
                            QStringLiteral("資料已成功匯出至：\n") + filename);
}

void MyWidget::on_pushButton_3_clicked()
{
    // 匯入按鈕 - Import data from file
    // Confirm before clearing existing data
    if (ui->tableWidget->rowCount() > 0) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, QStringLiteral("確認匯入"), 
                                     QStringLiteral("匯入將會清除現有資料。\n確定要繼續嗎？"),
                                     QMessageBox::Yes | QMessageBox::No);
        if (reply != QMessageBox::Yes) {
            return;
        }
    }
    
    QString filename = QFileDialog::getOpenFileName(this, 
        QStringLiteral("選擇檔案"), 
        QDir::homePath(),
        "Text Files (*.txt);;CSV Files (*.csv);;All Files (*)");
    
    if (filename.isEmpty()) {
        return; // User cancelled
    }
    
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, QStringLiteral("錯誤"), 
                            QStringLiteral("無法開啟檔案！"));
        return;
    }
    
    // Clear existing data
    ui->tableWidget->setRowCount(0);
    
    int importedCount = 0;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) {
            continue; // Skip empty lines
        }
        
        QStringList fields = line.split(",");
        
        if (fields.size() >= 4) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            
            for (int i = 0; i < 4 && i < fields.size(); i++) {
                QTableWidgetItem *item = new QTableWidgetItem(fields[i].trimmed());
                ui->tableWidget->setItem(row, i, item);
            }
            importedCount++;
        }
    }
    
    file.close();
    
    // Show result message
    if (importedCount > 0) {
        QMessageBox::information(this, QStringLiteral("成功"), 
                                QStringLiteral("已成功匯入 %1 筆聯絡人資料！").arg(importedCount));
    } else {
        QMessageBox::warning(this, QStringLiteral("警告"), 
                           QStringLiteral("檔案中沒有找到有效的聯絡人資料！"));
    }
}

void MyWidget::on_pushButton_4_clicked()
{
    // 結束按鈕 - Exit with confirmation
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QStringLiteral("確認結束"), 
                                 QStringLiteral("確定要結束程式嗎？\n請確認已匯出重要資料。"),
                                 QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        // Close the application
        QApplication::quit();
    }
}

