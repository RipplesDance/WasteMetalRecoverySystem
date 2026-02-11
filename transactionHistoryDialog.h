#ifndef TRANSACTIONHISTORYDIALOG_H
#define TRANSACTIONHISTORYDIALOG_H

#include <QDialog>
#include<QtDebug>
#include<QFile>
#include<QDir>
#include<QDataStream>
#include<QMessageBox>
#include"transaction.h"

namespace Ui {
class transactionHistoryDialog;
}

class transactionHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit transactionHistoryDialog(QWidget *parent = nullptr);
    ~transactionHistoryDialog();

    void init();
    void addTransactionToDialog(transaction data);

private:
    Ui::transactionHistoryDialog *ui;
};

#endif // TRANSACTIONHISTORYDIALOG_H
