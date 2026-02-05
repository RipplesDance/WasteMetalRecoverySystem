#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    fetchMetalPrice();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    ui->weight_line->setText(0);
    ui->final_price->setText(0);
    ui->weight_price->setText("未评估");
    ui->SOH_price->setText("未评估");
    ui->SOH_bar->setStyle(0);
    ui->SOH_bar->setRange(0,100);

    ui->type_line->addItem("磷酸铁锂电池");
    ui->type_line->addItem("钴酸锂电池");
    ui->type_line->addItem("三元锂离子电池");

    //connect signals
    connect(ui->SOH_bar, &QSlider::valueChanged,this, &MainWindow::onSlideValueChanged);
}

//SOH bar value changed slot
void MainWindow::onSlideValueChanged(int value)
{
    QString percentage =QString::number(value) + "%";
    ui->SOH_capcity->setText("剩余电池容量："+ percentage);
}

void MainWindow::fetchMetalPrice()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QString url = "https://api.metalpriceapi.com/v1/latest?api_key=89cd239feededd178eae6789fff5316b&base=CNY&currencies=XAG";
    connect(manager, &QNetworkAccessManager::finished, this, &MainWindow::fetchMetalPriceSlot);

    manager->get(QNetworkRequest(QUrl(url)));
}

void  MainWindow::fetchMetalPriceSlot(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(response);
        QJsonObject obj = doc.object();

        // {"success":true,"base":"CNY","timestamp":1770249599,"rates":{"CNYXAG":602.9848127058,"XAG":0.0016584166}}
        if (obj.contains("rates")) {
            QJsonObject ratesObj = obj["rates"].toObject();
                if (ratesObj.contains("CNYXAG")) {
                    double xagPrice = ratesObj["CNYXAG"].toDouble();// per ounce
                    double gramPrice = xagPrice / 31.1035;//per gram
                    QString priceStr = QString::number(gramPrice, 'f', 2) + "/g";
                    ui->co_price->setText(priceStr);
                } else {
                    qDebug() << "error: CNYXAG do not exist.";
                }
        }
    } else {
        qDebug() << "error:" << reply->errorString();
    }
    reply->deleteLater();

}
