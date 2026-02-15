#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QDebug>
#include<QMap>
#include<QRegularExpression>
#include<QMessageBox>
#include<QSettings>
#include<QUuid>
#include<QTimer>
#include<QFile>
#include<QDir>
#include<QDataStream>
#include <QTcpServer>
#include <QTcpSocket>
#include<QCloseEvent>
#include"batteryMaterialConcentration.h"
#include"quotation.h"
#include"transaction.h"
#include"transactionHistoryDialog.h"
#include"metalPrice.h"

enum {
    HANDSHAKE = 0,
    NEW_TRANSACTION = 1,
    TRANSACTION_STATUS = 2,
    METAL_PRICE = 3,
    QUOTATION_DATA = 4,
    HEART_BEAT=5
};

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
    double fetchNumberFromString(QString str);
    void makeDirPath(QString filePath);
    void sellButtonClicked(QString sellingWay);
    void frameClicked(QString frameType);
    void updateTransaction(transaction data);
    void newTransaction(transaction data);
    void sendMsgToServer(int type, transaction data);
    void sendMsgToServer(int type);
    void updateMetalPrice(metalPrice data);
    metalPrice readMetalPriceFromLocal();
    void saveMetalPriceToLocal(metalPrice data);

    void socketConnectToServer();
    QString getUUID();

    void startHandshake();

protected:
    void closeEvent(QCloseEvent *event) override;

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
