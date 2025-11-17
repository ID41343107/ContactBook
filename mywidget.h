#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MyWidget;
}
QT_END_NAMESPACE

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr);
    ~MyWidget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_edit_clicked();

    void on_lineEdit_search_textChanged(const QString &text);

    void on_tableWidget_itemSelectionChanged();

private:
    Ui::MyWidget *ui;
    void validateInputs();
    void clearInputs();
    void updateButtonStates();
    QString getDefaultFilePath();
};
#endif // MYWIDGET_H
