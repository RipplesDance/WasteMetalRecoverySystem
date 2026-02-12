#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QDebug>
#include<QMap>
#include<QRegularExpression>
#include<QMessageBox>
#include<QTimer>
#include<QFile>
#include<QDir>
#include<QDataStream>
#include <QTcpServer>
#include <QTcpSocket>
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
    void updateTransaction(transaction data);

    void socketConnectToServer();


public slots:
    void onSlideValueChanged(int value);
    void offFocus();
    void comboBoxchanged();

    void socketError(QAbstractSocket::SocketError socketError);
    void socketDisconnected();
    void socketConnected();

    void socketConnectingTimer_timeout();
    void connectBtnClicked();
    void msgFromServer();

signals:
    void newTransaction();


private:
    Ui::MainWindow *ui;
    QMap<QString, double> metalPriceMap;
    quotation quo;
    transactionHistoryDialog *transactionHistory_dialog;

    QTcpSocket *socket;
    bool isConnectted;
    QTimer *socketConnectingTimer;
    int times;
};
#endif // MAINWINDOW_H
