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
#include<QList>
#include<QMenu>
#include<QAction>
#include"interactableFrame.h"
#include"address.h"

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
    void init();
    void putAddressToUi(address data);
    void clearUi();
    void setAddress(address data);
    bool isUniqueDefault(address data);

public slots:
    void save_btn_clicked();
    void setDefaultRequested(address data);
    void deleteRequested(address data);

private:
    QVBoxLayout* m_contentLayout;
    Ui::addressDialog *ui;
    QMap<QString, QMap<QString, QStringList>> pcd_pair;
    QList<address> addressList;
};

#endif // ADDRESSDIALOG_H
