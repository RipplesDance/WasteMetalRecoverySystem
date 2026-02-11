#include "transaction.h"

transaction::transaction(QString type):type(type)
{
    isAccepted = false;
}
transaction::~transaction()
{

}

void transaction::setEnergyDensity(double energyDensity)
{
    this->energyDensity = energyDensity;
}

void transaction::setWeight(double weight)
{
    this->weight = weight;
}

void transaction::setSOH(double SOH)
{
    this->SOH = SOH;
}

void transaction::setPrice(double price)
{
    this->price = price;
}

void transaction::setTpye(QString type)
{
    this->type = type;
}

void transaction::setSellingWay(QString sellingWay)
{
    this->sellingWay = sellingWay;
}

void transaction::toogleAccept()
{
    isAccepted = !isAccepted;
}

void transaction::setUsagePurpose(QString usagePurpose)
{
    this->usagePurpose = usagePurpose;
}
 QDataStream &operator<<(QDataStream &out, const transaction &data)
 {
    out << data.classVersion << data.type << data.energyDensity << data.weight << data.SOH << data.price
        << data.usagePurpose << data.sellingWay << data.isAccepted;
    return out;
 }
QDataStream &operator>>(QDataStream &in, transaction &data)
{
    in >> data.classVersion;
    if(data.classVersion == 1)
    {
        in >> data.type >> data.energyDensity >> data.weight >> data.SOH >> data.price
                >> data.usagePurpose >> data.sellingWay >> data.isAccepted;
    }

    return in;
}
