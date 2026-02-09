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

void transaction::toogleAccept()
{
    isAccepted = !isAccepted;
}

void transaction::setUsagePurpose(QString usagePurpose)
{
    this->usagePurpose = usagePurpose;
}
