#ifndef ADDRESSDIALOG_H
#define ADDRESSDIALOG_H

#include <QDialog>
#include"address.h"
#include<QMap>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QCompleter>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include<QTimer>
#include<QVBoxLayout>
#include<QMessageBox>
#include"interactableFrame.h"

namespace Ui {
class addressDialog;
}

class addressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit addressDialog(QWidget *parent = nullptr);
    ~addressDialog();
    void parseAddressJson();
    void fetchLocationByIP();
    void parseLocationJson(const QByteArray &data);
    void autoSelectRegion(const QString &province, const QString &city);

public slots:
    void save_btn_clicked();

private:
    QVBoxLayout* m_contentLayout;
    Ui::addressDialog *ui;
    QMap<QString, QMap<QString, QStringList>> pcd_pair;
};

#endif // ADDRESSDIALOG_H
