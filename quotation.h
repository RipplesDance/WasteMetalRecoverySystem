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
    void init();

private:
    QMap<QString, batteryMaterialConcentration*> batteryMap;

public:
    double price_per_kilo; //include electricity fee， labor fee, and chemicals fee. Count in CNY.
    double profit;
};

#endif // QUOTATION_H
