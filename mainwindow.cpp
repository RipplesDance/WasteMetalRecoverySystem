#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    transactionHistory_dialog = new transactionHistoryDialog(this);
    transactionHistory_dialog->hide();

    socketConnectingTimer = new QTimer(this);
    times = 0;
    connect(socketConnectingTimer, &QTimer::timeout, this, &MainWindow::socketConnectingTimer_timeout);

    //socket init
    socket = new QTcpSocket(this);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MainWindow::socketError);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);
    connect(socket,&QTcpSocket::connected,this,&MainWindow::socketConnected);
    connect(socket, &QTcpSocket::readyRead,this,&MainWindow::msgFromServer);
    socketConnectToServer();

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
    connect(ui->connectBtn, &QPushButton::clicked, this, &MainWindow::connectBtnClicked);

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

    QDir dir;
    if(!dir.exists("bin/transactions"))
        makeDirPath("bin/transactions");
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
    if(!isConnectted)
    {
        QMessageBox::warning(this,"警告","未连接服务器，无法发送交易!");
        return;
    }

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
    transactionDetails.setUuid(getUUID());

    QString filePath = QString("bin/transactions/%1.dat").arg(transactionDetails.getId());
    transactionDetails.setFilePath(filePath);

    newTransaction(transactionDetails);

    transactionHistory_dialog->init();

    init();
}

void MainWindow::newTransaction(transaction data)
{
    QFile file(data.selectFilePath());
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "错误", "无法读取本地文件！");
        return;
    }

    //start out
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_14);
    out << data;

    file.close();
    sendMsgToServer(NEW_TRANSACTION, data);

}

void MainWindow::sendMsgToServer(int type, transaction data)
{
    QByteArray block;
    QDataStream out_server(&block, QIODevice::WriteOnly);
    out_server << type << data;
    socket->write(block);
    socket->flush(); //
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

void MainWindow::updateTransaction(transaction data)
{
    QFile file(data.selectFilePath());
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "错误", "无法更新订单状态！");
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_14);
    out << data;
    file.close();
}

QString MainWindow::getUUID() {
    // 存储在程序根目录下的 config.ini 中
    QSettings settings("config.ini", QSettings::IniFormat);
    QString uuid = settings.value("Device/UUID").toString();

    if (uuid.isEmpty()) {
        // 第一次运行，生成新的 UUID
        uuid = QUuid::createUuid().toString(); // 格式如: {7b3c...}
        settings.setValue("Device/UUID", uuid);
    }
    return uuid;
}

void MainWindow::startHandshake()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_14);

    out << HANDSHAKE;
    out << getUUID();

    socket->write(block);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
    }

        QMessageBox::StandardButton resBtn = QMessageBox::question(this, "确认", "确定要退出程序吗？",
                                                                   QMessageBox::No | QMessageBox::Yes,
                                                                   QMessageBox::Yes);
        if (resBtn != QMessageBox::Yes) {
            event->ignore();
            return;
        }

    event->accept();
}

//socket-server function
void MainWindow::socketError(QAbstractSocket::SocketError socketError)
{
    socketConnectingTimer->stop();
    times = 0;
    QMessageBox::critical(this,"错误",QString("连接远程服务器失败！错误代码:%1").arg(socketError));
    ui->socketStatus_label->setText("未连接");
    ui->connectBtn->setEnabled(true);
    ui->connectBtn->setText("重新连接");
}

void MainWindow::socketDisconnected()
{
    ui->socketStatus_label->setText("未连接");
    ui->connectBtn->setText("重新连接");

}

void MainWindow::socketConnected()
{
    startHandshake();
    isConnectted = true;
    ui->socketStatus_label->setText("已连接");
    ui->connectBtn->setText("断开连接");
    ui->connectBtn->setEnabled(true);
    socketConnectingTimer->stop();
//    QMessageBox::information(this,"成功","连接服务器成功！");
}

void MainWindow::socketConnectToServer()
{
    isConnectted = false;
    socketConnectingTimer->start(1000);
    socket->connectToHost("127.0.0.1", 8888);
    ui->connectBtn->setEnabled(false);
}

void MainWindow::socketConnectingTimer_timeout()
{
    times++;
    if(times > 4)
        times = 0;
    else if (times == 1)
        ui->socketStatus_label->setText("连接中");
    else if (times == 2)
        ui->socketStatus_label->setText("连接中.");
    else if (times == 3)
        ui->socketStatus_label->setText("连接中..");
    else if (times == 4)
        ui->socketStatus_label->setText("连接中...");
}

void MainWindow::connectBtnClicked()
{
    if(ui->connectBtn->text() == "重新连接")
        socketConnectToServer();
    else if(ui->connectBtn->text() == "断开连接")
    {
        QMessageBox::critical(this,"警告","服务器断开连接！");
        socket->disconnectFromHost();
    }
}

void MainWindow::msgFromServer()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_14);
    while(true)
    {
        in.startTransaction();

        int order;
        in>>order;

        if(order == NEW_TRANSACTION)//transaction received
            QMessageBox::information(this, "成功", "电池交易请求提交成功！");
        else if(order == TRANSACTION_STATUS)//transaction status updated
        {
            transaction data;
            in>>data;
            if(in.commitTransaction())
            {
                updateTransaction(data);
                transactionHistory_dialog->init();
            }

        }
        else if(order == METAL_PRICE)
        {
            //
        }
        else
        {
            in.rollbackTransaction();
            break;
        }
    }


}
