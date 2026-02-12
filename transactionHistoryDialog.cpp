#include "transactionHistoryDialog.h"
#include "ui_transactionHistoryDialog.h"

transactionHistoryDialog::transactionHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::transactionHistoryDialog)
{
    ui->setupUi(this);

    ui->sort_box->addItem("按订单创建时间正序");
    ui->sort_box->addItem("按订单创建时间倒序");
    ui->sort_box->addItem("按报价大小正序");
    ui->sort_box->addItem("按报价大小倒序");

    connect(ui->sort_box, &QComboBox::currentTextChanged, this, &transactionHistoryDialog::sortBoxChanged);
    connect(ui->transactionList, &QListWidget::itemClicked, this, &transactionHistoryDialog::selectedItem);

    init();
}

transactionHistoryDialog::~transactionHistoryDialog()
{
    delete ui;
}

void transactionHistoryDialog::init()
{
    QDir dir("bin/transactions");

    //set filter
    QStringList filter;
    filter<< "*.dat";
    dir.setNameFilters(filter);

    //start reading file
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotDot);

    fileVector.clear();
    for(const QFileInfo &fileInfo : fileList)
    {
        QFile file(fileInfo.absoluteFilePath());
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this,"错误","文件无法读取!");
            return;
        }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_14);
        transaction data;
        in >> data;
        fileVector.push_back(data);

        file.close();
    }

    sortBoxChanged(ui->sort_box->currentText());
}

void transactionHistoryDialog::updataTransaction(transaction data)
{
    ui->type_label->setText(data.selectType());
    ui->price_label->setText(QString::number(data.selectPrice(),'f', 2) + "元");
    ui->id_label->setText("id:" + data.getId());

    ui->weight_label->setText("重量:" + QString::number(data.selectWeight(),'f', 2) + "kg");
    ui->SOH_label->setText("SOH:" + QString::number(data.selectSOH()*100) + "%");
    ui->energyDensity_label->setText("能量密度:" + QString::number(data.selectEnergyDensity(),'f', 2) + "Wh/kg");
    ui->leagcyElectricity_label->setText("剩余电量:" + QString::number(data.selectLeagcyElectricity(),'f', 2) + "度");
    ui->usage_label->setText("回收用途:" + data.selectUsagePurpose());
    ui->sellingWay_label->setText(QString("出售方式:%1")
                                  .arg(data.selectSellingWay() == "offline" ? "上门回收" : "线上邮寄"));

    QDateTime submit = data.selectSubmittedTime();
    QDateTime result = data.selectResultTime();
    if(!submit.isValid())
    {
        QMessageBox::warning(this,"警告","无法获取交易时间");
        return;
    }
    ui->submittedTime_label->setText(submit.toString("yyyy-MM-dd hh:mm"));
    if(!result.isValid())
    {
        ui->transactionStatus_label->setText("交易状态:处理中");
        return;
    }
    ui->transactionStatus_label->setText(QString("交易状态:%1").arg(data.checkStatus()? "已完成" : "被拒绝"));
    ui->resultTime_label->setText(result.toString("yyyy-MM-dd hh:mm"));

    qint64 totalSecs = submit.secsTo(result);
    int hours = totalSecs / 3600;
    int minutes = (totalSecs % 3600) / 60;
    ui->duration_label->setText(QString("耗时:%1时%2分").arg(hours, minutes));


}

void transactionHistoryDialog::onNewTransaction()
{
    init();
}

void transactionHistoryDialog::selectedItem(QListWidgetItem *item)
{
    if(!item) return;
    QString filePath = item->data(Qt::UserRole).toString();
    qDebug()<<filePath;

    auto it = std::find_if(fileVector.begin(), fileVector.end(), [=](transaction d){
            return d.selectFilePath() == filePath;
        });

        if (it != fileVector.end()) {
            transaction clickedData = *it;
            updataTransaction(clickedData);
        }
}

void transactionHistoryDialog::updataListWidget()
{
    ui->transactionList->clear();
    for(auto data : fileVector)
    {
        QListWidgetItem* item = new QListWidgetItem(data.selectType() + "-" +
                                                    QString::number(data.selectPrice()) + "-" + data.getId());
        item->setData(Qt::UserRole, data.selectFilePath());
        ui->transactionList->addItem(item);
    }
}

void transactionHistoryDialog::sortBoxChanged(QString way)
{
    if(way == "按订单创建时间正序")
    {
        std::sort(fileVector.begin(), fileVector.end(),
                  [=](transaction &a, transaction &b){return a.selectSubmittedTime() > b.selectSubmittedTime();});
    }
    else if(way == "按订单创建时间倒序")
    {
        std::sort(fileVector.begin(), fileVector.end(),
                  [=](transaction &a, transaction &b){return a.selectSubmittedTime() < b.selectSubmittedTime();});
    }
    else if(way == "按报价大小正序")
    {
        std::sort(fileVector.begin(), fileVector.end(),
                  [=](transaction &a, transaction &b){return a.selectPrice() > b.selectPrice();});
    }
    else if(way == "按报价大小倒序")
    {
        std::sort(fileVector.begin(), fileVector.end(),
                  [=](transaction &a, transaction &b){return a.selectPrice() < b.selectPrice();});
    }

    updataListWidget();
}
