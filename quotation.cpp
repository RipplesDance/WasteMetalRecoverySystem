#include "quotation.h"

quotation::quotation()
{
    init();

    batteryMaterialConcentration* LCO = new batteryMaterialConcentration(0.071,0.602,0,0,0.1, 0.93, 0.35);
    LCO->setRecycleRatio(0.85, 0.95,0,0,0.98,0.95);
    LCO->setProperty(150,0.3, 0.35);
    batteryMap.insert("钴酸锂电池", LCO);
}

quotation:: ~quotation()
{
}

void quotation::init()
{
   price_per_kilo = 15;
   profit = 0.2;
}

double quotation::quotationCaculator(QString type, int weight, double SOH, QMap<QString, double> metalPriceMap)
{
    if(!batteryMap.contains(type))
        return 0.0;
    batteryMaterialConcentration* battery = batteryMap.value(type);

    if(SOH >= 0.8)
    {
        //reusable
        double finalPrice = weight * battery->energyDensity * SOH * 0.75;
        if(SOH>=0.9)
            return finalPrice * battery->unitPrice_90;
        return finalPrice * battery->unitPrice_80;
    }

    double positiveMaterial = weight * battery->positiveMaterialsRatio * battery->positiveMaterial_recycleRatio;
    double positiveMaterialCompound = positiveMaterial * battery->compoundRatio;

    qDebug()<<positiveMaterial << positiveMaterialCompound;

    //metal quotation
    double li_quotation =  positiveMaterialCompound * battery->li *
             battery->li_recycleRatio * metalPriceMap.value("Li");
    double co_quotation = positiveMaterialCompound * battery->co *
             battery->co_recycleRatio * metalPriceMap.value("Co");
    double mn_quotation = positiveMaterialCompound * battery->mn *
             battery->mn_recycleRatio * metalPriceMap.value("Mn");
    double ni_quotation = positiveMaterialCompound * battery->ni *
            battery->ni_recycleRatio * metalPriceMap.value("Ni");

    double cu_quotation = weight * battery->cu * battery->cu_recycleRatio * metalPriceMap.value("Cu");

    qDebug()<<li_quotation << co_quotation<<mn_quotation<<ni_quotation<<cu_quotation;

    //final price
    double finalPrice = (li_quotation+co_quotation+mn_quotation+ni_quotation+cu_quotation)
            - battery->otherCost - price_per_kilo * weight;

    return finalPrice > 0 ? finalPrice* (1 - profit) : 0;

}
