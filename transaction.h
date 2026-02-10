#ifndef TRANSACTION_H
#define TRANSACTION_H

#include<QtDebug>
#include<QDataStream>
#include <QString>

class transaction
{
public:
    transaction(QString type);
    ~transaction();
    void setEnergyDensity(double energyDensity);
    void setWeight(double weight);
    void setSOH(double SOH);
    void setPrice(double price);
    void setTpye(QString type);
    void setUsagePurpose(QString usagePurpose);
    void toogleAccept();

private:
    QString type;
    QString usagePurpose;
    double energyDensity;
    double weight;
    double SOH;
    double price;
    bool isAccepted;

    int classVersion = 1;


    friend QDataStream &operator<<(QDataStream &out, const transaction &data);
    friend QDataStream &operator>>(QDataStream &in, transaction &data);

};

//QDataStream &operator<<(QDataStream &out, const transaction &data);
// QDataStream &operator>>(QDataStream &in, transaction &data);

#endif // TRANSACTION_H
