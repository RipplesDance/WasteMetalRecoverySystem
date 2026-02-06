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
    connect(ui->weight_line, &QLineEdit::editingFinished, this, &MainWindow::offFocus);
    connect(ui->SOH_bar, &QSlider::sliderReleased, this, &MainWindow::offFocus);
    connect(ui->type_line, &QComboBox::currentTextChanged, this, &MainWindow::offFocus);
}

//SOH bar value changed slot
void MainWindow::onSlideValueChanged(int value)
{
    QString percentage =QString::number(value) + "%";
    ui->SOH_capcity->setText("剩余电池容量："+ percentage);
}

void MainWindow::offFocus()
{
    QString capcity = ui->SOH_capcity->text();
    capcity.remove(0,7);//only percentage
    QString weight = ui->weight_line->text();
    QString type = ui->type_line->currentText();

    qDebug()<<capcity.isEmpty() << weight.isEmpty() << type;

    if(weight.isEmpty() || capcity.isEmpty())
        return;

    qDebug()<<"可以计算";
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
    QString fe_str = ui->fe_price->text();

    metalPriceMap.insert("Li", fetchNumberFromString(li_str));
    metalPriceMap.insert("Co", fetchNumberFromString(co_str));
    metalPriceMap.insert("Mn", fetchNumberFromString(mn_str));
    metalPriceMap.insert("Ni", fetchNumberFromString(ni_str));
    metalPriceMap.insert("Fe", fetchNumberFromString(fe_str));
    qDebug()<<metalPriceMap;
}
