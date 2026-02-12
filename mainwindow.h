#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QDebug>
#include<QMap>
#include<QRegularExpression>
#include<QMessageBox>
#include<QFile>
#include<QDir>
#include<QDataStream>
#include"batteryMaterialConcentration.h"
#include"quotation.h"
#include"transaction.h"
#include"transactionHistoryDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();
    void getMetalPrice();
    double fetchNumberFromString(QString str);
    void makeDirPath(QString filePath);
    void sellButtonClicked(QString sellingWay);
    void frameClicked(QString frameType);

private slots:
    void onSlideValueChanged(int value);
    void offFocus();
    void comboBoxchanged();

signals:
    void newTransaction();


private:
    Ui::MainWindow *ui;
    QMap<QString, double> metalPriceMap;
    quotation quo;
    transactionHistoryDialog *transactionHistory_dialog;
};
#endif // MAINWINDOW_H
