#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
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
}
