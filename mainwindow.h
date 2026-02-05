#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void fetchMetalPrice();

    //slots
private slots:
    void onSlideValueChanged(int value);
    void fetchMetalPriceSlot(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
