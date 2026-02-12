#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    transactionHistory_dialog = new transactionHistoryDialog(this);
    transactionHistory_dialog->hide();
    connect(this, &MainWindow::newTransaction, transactionHistory_dialog,
            &transactionHistoryDialog::onNewTransaction);

    ui->type_line->addItem("钴酸锂电池");
    ui->type_line->addItem("磷酸铁锂电池");
    ui->type_line->addItem("三元锂离子电池");

    //connect signals
    connect(ui->SOH_bar, &QSlider::valueChanged,this, &MainWindow::onSlideValueChanged);
    connect(ui->weight_spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::offFocus);
    connect(ui->SOH_bar, &QSlider::sliderReleased, this, &MainWindow::offFocus);
    connect(ui->energyDensity_spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::offFocus);
    connect(ui->type_line, &QComboBox::currentTextChanged, this, &MainWindow::comboBoxchanged);

    connect(ui->transactionHistory_frame, &interactableFrame::clicked,[=](){this->frameClicked("transactionHistory");});
    connect(ui->batteryInfo_frame, &interactableFrame::clicked,[=](){this->frameClicked("batteryInfo");});

    connect(ui->sellButton_offline, &QPushButton::clicked, [=](){sellButtonClicked("offline");});
    connect(ui->sellButton_online, &QPushButton::clicked, [=](){sellButtonClicked("online");});

    //label cannot block mouse release
    ui->transactionHistory_label->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->batteryInfo_label->setAttribute(Qt::WA_TransparentForMouseEvents);

    init();
    getMetalPrice();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    ui->weight_spinBox->setValue(0.0);
    ui->energyDensity_spinBox->setValue(0.0);
    ui->final_price->setText(0);
    ui->usagePurpose->setText("未评估");
    ui->leagcyElectricity->setText("未评估");
    ui->SOH_bar->setValue(0);
    ui->SOH_bar->setRange(0,100);
}

//SOH bar value changed slot
void MainWindow::onSlideValueChanged(int value)
{
    QString percentage =QString::number(value) + "%";
    ui->SOH_capcity->setText("剩余电池容量："+ percentage);
}
//combo box value changed
void MainWindow::comboBoxchanged()
{
    init();
}
//sell button clicked
void MainWindow::sellButtonClicked(QString sellingWay)
{
     double weight = ui->weight_spinBox->value();
     QString text_SOH = ui->SOH_capcity->text();

    if(weight <=0.0 || text_SOH.isEmpty())
    {
        QMessageBox::warning(this, "提示", "请先评估价格");
        return;
    }

    double energyDensity = ui->energyDensity_spinBox->value();
    QString type = ui->type_line->currentText();
    double SOH = fetchNumberFromString(text_SOH) / 100;

    QString text_price = ui->final_price->text();
    text_price.chop(1);
    double price = fetchNumberFromString(text_price);

    QString usagePurpose = ui->usagePurpose->text();
    double leagcyElectricity = energyDensity * SOH * weight;

    //creat transaction class
    transaction transactionDetails(type);
    transactionDetails.setSOH(SOH);
    transactionDetails.setPrice(price);
    transactionDetails.setWeight(weight);
    transactionDetails.setEnergyDensity(energyDensity);
    transactionDetails.setUsagePurpose(usagePurpose);
    transactionDetails.setSellingWay(sellingWay);
    transactionDetails.setLeagcyElectricity(leagcyElectricity);

    //out preparation
    QDir dir;
    if(!dir.exists("bin/transactions"))
        makeDirPath("bin/transactions");

    QString filePath = QString("bin/transactions/%1.dat").arg(transactionDetails.getId());
    transactionDetails.setFilePath(filePath);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "错误", "无法读取本地文件！");
        return;
    }

    //start out
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_14);
    out << transactionDetails;

    file.close();

    emit newTransaction();
    QMessageBox::information(this, "成功", "电池交易请求提交成功！");

    init();
}

void MainWindow::makeDirPath(QString filePath)
{
    QDir dir;
    if (dir.mkpath(filePath)) {
        qDebug() << "Dir created";
    }

}

void MainWindow::offFocus()
{
    QString text_SOH = ui->SOH_capcity->text();

    double weight = ui->weight_spinBox->value();

    double energyDensity = ui->energyDensity_spinBox->value();

    QString type = ui->type_line->currentText();



    if(weight <=0.0 || text_SOH.isEmpty())
    {
        ui->final_price->setText(0);
        return;
    }


    double SOH = fetchNumberFromString(text_SOH) / 100;
    double leagcyElectricity = energyDensity * SOH * weight;
    if(SOH >= 0.8 && energyDensity >0)
        ui->usagePurpose->setText("梯次回收利用");
    else
        ui->usagePurpose->setText("金属回收");

    if(energyDensity > 0 && SOH > 0 && weight>0)
    {
        ui->leagcyElectricity->setText(QString("剩余%1度电").arg(leagcyElectricity, 0, 'f', 2));
    }

    double finalPrice = quo.quotationCaculator(type, energyDensity, weight, SOH, metalPriceMap);

    QString str = QString::number(finalPrice,'f', 2);

    ui->final_price->setText(str + "元");
}

double MainWindow::fetchNumberFromString(QString str)
{
    QRegularExpression re("\\d+\\.?\\d*");
    QRegularExpressionMatch match = re.match(str);
    double price = 0.00;

    if (match.hasMatch()) {
        QString result = match.captured(0);
        price = result.toDouble();
    }
    return price;
}

void MainWindow::getMetalPrice()
{
    QString li_str = ui->li_price->text();
    QString co_str = ui->co_price->text();
    QString mn_str = ui->mn_price->text();
    QString ni_str = ui->ni_price->text();
    QString cu_str = ui->cu_price->text();

    metalPriceMap.insert("Li", fetchNumberFromString(li_str)/ 1000);
    metalPriceMap.insert("Co", fetchNumberFromString(co_str)/ 1000);
    metalPriceMap.insert("Mn", fetchNumberFromString(mn_str)/ 1000);
    metalPriceMap.insert("Ni", fetchNumberFromString(ni_str)/ 1000);
    metalPriceMap.insert("Cu", fetchNumberFromString(cu_str)/ 1000);
}

void MainWindow::frameClicked(QString frameType)
{
    if(frameType == "transactionHistory")
    {
        transactionHistory_dialog->show();
    }
}
