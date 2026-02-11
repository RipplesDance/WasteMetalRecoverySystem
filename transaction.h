#ifndef TRANSACTION_H
#define TRANSACTION_H

#include<QtDebug>
#include<QDataStream>
#include <QString>
#include<QDateTime>

class transaction
{
public:
    transaction(QString type = NULL);
    ~transaction();
    void setEnergyDensity(double energyDensity);
    void setWeight(double weight);
    void setSOH(double SOH);
    void setPrice(double price);
    void setTpye(QString type);
    void setUsagePurpose(QString usagePurpose);
    void setSellingWay(QString sellingWay);


protected:
    void setSubmittedTime(QDateTime submittedTime);
    void toogleAccept();

    void changeClassVersion(int version);

private:
    QString type;
    QString usagePurpose;
    QString sellingWay;
    double energyDensity;
    double weight;
    double SOH;
    double price;

    bool isAccepted;

    QDateTime submittedTime;
    QDateTime resultTime;


    int classVersion;


    friend QDataStream &operator<<(QDataStream &out, const transaction &data);
    friend QDataStream &operator>>(QDataStream &in, transaction &data);

};

//QDataStream &operator<<(QDataStream &out, const transaction &data);
// QDataStream &operator>>(QDataStream &in, transaction &data);

#endif // TRANSACTION_H
