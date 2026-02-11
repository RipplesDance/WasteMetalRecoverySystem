#include "transactionhistoryWidget.h"
#include "ui_transactionhistoryWidget.h"

transactionHistoryWidget::transactionHistoryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::transactionHistoryWidget)
{
    ui->setupUi(this);
}

transactionHistoryWidget::~transactionHistoryWidget()
{
    delete ui;
}
