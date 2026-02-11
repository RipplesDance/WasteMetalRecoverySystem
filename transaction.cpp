#include "transaction.h"

transaction::transaction(QString type):type(type)
{
    isAccepted = false;
    changeClassVersion(1);
    setSubmittedTime(QDateTime::currentDateTime());
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
    this->resultTime = QDateTime::currentDateTime();
}

void transaction::setUsagePurpose(QString usagePurpose)
{
    this->usagePurpose = usagePurpose;
}

void transaction::setSubmittedTime(QDateTime submittedTime)
{
    this->submittedTime = submittedTime;
}

void transaction::changeClassVersion(int version)
{
    this->classVersion = version;
}

 QDataStream &operator<<(QDataStream &out, const transaction &data)
 {

    out << data.classVersion << data.type << data.energyDensity << data.weight << data.SOH << data.price
        << data.usagePurpose << data.sellingWay << data.isAccepted
        << data.submittedTime << data.resultTime;
    return out;
 }

QDataStream &operator>>(QDataStream &in, transaction &data)
{

    in >> data.classVersion;
    if(data.classVersion == 1)
    {
        in >> data.type >> data.energyDensity >> data.weight >> data.SOH >> data.price
                >> data.usagePurpose >> data.sellingWay >> data.isAccepted
                >> data.submittedTime >> data.resultTime;
    }

    return in;
}
