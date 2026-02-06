#ifndef QUOTATION_H
#define QUOTATION_H

#include<QDebug>
#include<QMap>
#include"batteryMaterialConcentration.h"

class quotation
{

public:
    quotation();
    ~quotation();
    double quotationCaculator(QString type, int weight, double SOH, QMap<QString, double> metalPriceMap);

private:
    QMap<QString, batteryMaterialConcentration*> batteryMap;

public:
    double price_per_kilo = 15;
};

#endif // QUOTATION_H
