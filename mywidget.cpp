#include "mywidget.h"
#include "ui_mywidget.h"

#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QRegularExpression>
#include <QShortcut>

QString mFilename = "";

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

    // Initialize default file path
    mFilename = getDefaultFilePath();

    QStringList ColTotle;

    ui->tableWidget->setColumnCount (4);
    ColTotle<<QStringLiteral ("學號")<<QStringLiteral ("班級")<<QStringLiteral ("姓名")<<QStringLiteral ("電話");
    ui->tableWidget->setHorizontalHeaderLabels (ColTotle);
    
    // Set column widths to stretch proportionally
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setColumnWidth(0, 120);
    ui->tableWidget->setColumnWidth(1, 120);
    ui->tableWidget->setColumnWidth(2, 120);
    
    // Enable sorting
    ui->tableWidget->setSortingEnabled(true);
    
    // Set up keyboard shortcuts
    QShortcut *addShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_N), this);
    connect(addShortcut, &QShortcut::activated, this, &MyWidget::on_pushButton_clicked);
    
    QShortcut *deleteShortcut = new QShortcut(QKeySequence::Delete, this);
    connect(deleteShortcut, &QShortcut::activated, this, &MyWidget::on_pushButton_delete_clicked);
    
    QShortcut *saveShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    connect(saveShortcut, &QShortcut::activated, this, &MyWidget::on_pushButton_2_clicked);
    
    // Update button states on startup
    updateButtonStates();
    
    // Set focus to first input
    ui->lineEdit->setFocus();
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::on_pushButton_clicked()
{
    // Validate inputs - at least student ID and name are required
    if (ui->lineEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("輸入錯誤"), 
                           QStringLiteral("請輸入學號"));
        ui->lineEdit->setFocus();
        return;
    }
    
    if (ui->lineEdit_3->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QStringLiteral("輸入錯誤"), 
                           QStringLiteral("請輸入姓名"));
        ui->lineEdit_3->setFocus();
        return;
    }

    QTableWidgetItem *inputRow1,*inputRow2,*inputRow3,*inputRow4;

    inputRow1= new QTableWidgetItem(QString (ui->lineEdit->text().trimmed()));
    inputRow2= new QTableWidgetItem(QString(ui->lineEdit_2->text().trimmed()));
    inputRow3= new QTableWidgetItem(QString (ui->lineEdit_3->text().trimmed()));
    inputRow4= new QTableWidgetItem(QString (ui->lineEdit_4->text().trimmed()));

    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->insertRow (ui->tableWidget->rowCount());

    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,inputRow1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1,inputRow2);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,2,inputRow3);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,3,inputRow4);
    
    ui->tableWidget->setSortingEnabled(true);
    
    // Clear inputs after adding
    clearInputs();
    ui->lineEdit->setFocus();
    
    ui->statusBar->showMessage(QStringLiteral("已新增聯絡人"), 3000);
}


void MyWidget::on_pushButton_2_clicked()
{
    if (ui->tableWidget->rowCount() == 0) {
        QMessageBox::information(this, QStringLiteral("提示"), 
                                QStringLiteral("沒有資料可以匯出"));
        return;
    }
    
    QString filename = QFileDialog::getSaveFileName(this, 
        QStringLiteral("匯出資料"), 
        mFilename, 
        "CSV Files (*.csv);;Text Files (*.txt);;All Files (*)");
    
    if (filename.isEmpty()) {
        return; // User cancelled
    }
    
    QString saveFile=""; //存檔內容
    for(int i = 0 ;i<ui->tableWidget->rowCount();i++) {
        for (int j=0;j<ui->tableWidget->columnCount();j++) {
            if (ui->tableWidget->item(i,j)) {
                saveFile+=ui->tableWidget->item(i,j)->text()+",";
            } else {
                saveFile+=",";
            }
        }
        saveFile+="\n";
    }
    Write (filename, saveFile); //呼叫Write
    ui->statusBar->showMessage(QStringLiteral("資料已匯出至：") + filename, 5000);
}

void MyWidget::on_pushButton_3_clicked()
{
    // 匯入按鈕 - Import data from file
    QString filename = QFileDialog::getOpenFileName(this, 
        QStringLiteral("選擇檔案"), 
        QDir::homePath(), 
        "CSV Files (*.csv);;Text Files (*.txt);;All Files (*)");
    
    if (filename.isEmpty()) {
        return; // User cancelled
    }
    
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QStringLiteral("錯誤"), QStringLiteral("無法開啟檔案"));
        return;
    }
    
    // Ask user if they want to append or replace
    QMessageBox::StandardButton reply = QMessageBox::NoButton;
    if (ui->tableWidget->rowCount() > 0) {
        reply = QMessageBox::question(this, QStringLiteral("匯入選項"), 
                                      QStringLiteral("要覆蓋現有資料還是附加到現有資料？\n\n是：覆蓋\n否：附加"),
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (reply == QMessageBox::Cancel) {
            file.close();
            return;
        }
    }
    
    // Clear existing data if user chose to replace
    if (reply == QMessageBox::Yes || ui->tableWidget->rowCount() == 0) {
        ui->tableWidget->setRowCount(0);
    }
    
    ui->tableWidget->setSortingEnabled(false);
    
    int importedCount = 0;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        
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
    
    ui->tableWidget->setSortingEnabled(true);
    file.close();
    
    ui->statusBar->showMessage(QStringLiteral("已匯入 %1 筆資料").arg(importedCount), 5000);
}

void MyWidget::on_pushButton_4_clicked()
{
    // 結束按鈕 - Save data and exit
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QStringLiteral("確認"), 
                                  QStringLiteral("是否儲存並結束程式？"),
                                  QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    
    if (reply == QMessageBox::Cancel) {
        return;
    }
    
    if (reply == QMessageBox::Yes) {
        // Save the data
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
    }
    
    // Close the application
    QApplication::quit();
}

void MyWidget::on_pushButton_clear_clicked()
{
    clearInputs();
    ui->lineEdit->setFocus();
}

void MyWidget::on_pushButton_delete_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, QStringLiteral("警告"), 
                           QStringLiteral("請先選擇要刪除的項目"));
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, QStringLiteral("確認刪除"), 
                                  QStringLiteral("確定要刪除選取的項目嗎？"),
                                  QMessageBox::Yes|QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        ui->tableWidget->removeRow(currentRow);
        ui->statusBar->showMessage(QStringLiteral("已刪除項目"), 3000);
    }
    
    updateButtonStates();
}

void MyWidget::on_pushButton_edit_clicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, QStringLiteral("警告"), 
                           QStringLiteral("請先選擇要編輯的項目"));
        return;
    }
    
    // Load data from selected row into input fields
    if (ui->tableWidget->item(currentRow, 0)) {
        ui->lineEdit->setText(ui->tableWidget->item(currentRow, 0)->text());
    }
    if (ui->tableWidget->item(currentRow, 1)) {
        ui->lineEdit_2->setText(ui->tableWidget->item(currentRow, 1)->text());
    }
    if (ui->tableWidget->item(currentRow, 2)) {
        ui->lineEdit_3->setText(ui->tableWidget->item(currentRow, 2)->text());
    }
    if (ui->tableWidget->item(currentRow, 3)) {
        ui->lineEdit_4->setText(ui->tableWidget->item(currentRow, 3)->text());
    }
    
    // Remove the row (user can add it again with updated data)
    ui->tableWidget->removeRow(currentRow);
    ui->lineEdit->setFocus();
    ui->statusBar->showMessage(QStringLiteral("編輯模式：修改資料後請點擊新增"), 5000);
}

void MyWidget::on_lineEdit_search_textChanged(const QString &text)
{
    // Filter table based on search text
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        bool match = false;
        if (text.isEmpty()) {
            match = true;
        } else {
            for (int j = 0; j < ui->tableWidget->columnCount(); ++j) {
                if (ui->tableWidget->item(i, j)) {
                    if (ui->tableWidget->item(i, j)->text().contains(text, Qt::CaseInsensitive)) {
                        match = true;
                        break;
                    }
                }
            }
        }
        ui->tableWidget->setRowHidden(i, !match);
    }
}

void MyWidget::on_tableWidget_itemSelectionChanged()
{
    updateButtonStates();
}

void MyWidget::validateInputs()
{
    // This can be expanded for more validation
}

void MyWidget::clearInputs()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
}

void MyWidget::updateButtonStates()
{
    bool hasSelection = ui->tableWidget->currentRow() >= 0;
    ui->pushButton_delete->setEnabled(hasSelection);
    ui->pushButton_edit->setEnabled(hasSelection);
}

QString MyWidget::getDefaultFilePath()
{
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir dir(documentsPath);
    if (!dir.exists("ContactBook")) {
        dir.mkdir("ContactBook");
    }
    return dir.filePath("ContactBook/contacts.csv");
}

