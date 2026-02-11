#include "transactionHistoryDialog.h"
#include "ui_transactionHistoryDialog.h"

transactionHistoryDialog::transactionHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::transactionHistoryDialog)
{
    ui->setupUi(this);
}

transactionHistoryDialog::~transactionHistoryDialog()
{
    delete ui;
}

void transactionHistoryDialog::init()
{
    QDir dir("bin/transacitonHistory");
    if(!dir.exists())
    {
        //creat dir
        return;
    }

    //set filter
    QStringList filter;
    filter<< "*.dat";
    dir.setNameFilters(filter);

    //start reading file
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotDot);

    for(const QFileInfo &fileInfo : fileList)
    {
        QFile file(fileInfo.absolutePath());

        if(!file.open(QFile::ReadOnly))
            continue;

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_14);

        transaction data;
        in >> data;
        file.close();

        addTransactionToDialog(data);
    }
}

void transactionHistoryDialog::addTransactionToDialog(transaction data)
{

}
