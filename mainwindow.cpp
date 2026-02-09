#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    ui->weight_price->setText("未评估");
    ui->SOH_price->setText("未评估");
    ui->SOH_bar->setStyle(0);
    ui->SOH_bar->setRange(0,100);

    ui->type_line->addItem("钴酸锂电池");
    ui->type_line->addItem("磷酸铁锂电池");
    ui->type_line->addItem("三元锂离子电池");

    //connect signals
    connect(ui->SOH_bar, &QSlider::valueChanged,this, &MainWindow::onSlideValueChanged);
    connect(ui->weight_spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::offFocus);
    connect(ui->SOH_bar, &QSlider::sliderReleased, this, &MainWindow::offFocus);
    connect(ui->type_line, &QComboBox::currentTextChanged, this, &MainWindow::offFocus);
    connect(ui->energyDensity_spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::offFocus);
}

//SOH bar value changed slot
void MainWindow::onSlideValueChanged(int value)
{
    QString percentage =QString::number(value) + "%";
    ui->SOH_capcity->setText("剩余电池容量："+ percentage);
}

void MainWindow::offFocus()
{
    QString text_SOH = ui->SOH_capcity->text();

    double weight = ui->weight_spinBox->value();

    double energyDensity = ui->energyDensity_spinBox->value();

    QString type = ui->type_line->currentText();

    if(weight <=0.0 || text_SOH.isEmpty())
        return;

    double SOH = fetchNumberFromString(text_SOH) / 100;

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
    qDebug()<<metalPriceMap;
}
