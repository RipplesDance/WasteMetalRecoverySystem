#include "quotation.h"

quotation::quotation()
{
    batteryMap.insert("钴酸锂电池", new batteryMaterialConcentration(0.6,0.07,0,0,0.1, 0.93, 0.4));
}

quotation:: ~quotation()
{
}

double quotation::quotationCaculator(QString type, int weight, double SOH, QMap<QString, double> metalPriceMap)
{
    if(!batteryMap.contains(type))
        return 0.0;
    batteryMaterialConcentration* battery = batteryMap.value(type);

    if(SOH > 80.0)
    {
        //reusable
    }

    //metal quotation
    double li_quotation = weight * battery->positiveMaterialsRatio * battery->li *
            battery->compoundRatio * battery->li_recycleRatio * metalPriceMap.value("Li");
    double co_quotation = weight * battery->positiveMaterialsRatio * battery->co *
            battery->compoundRatio * battery->co_recycleRatio * metalPriceMap.value("Co");
    double mn_quotation = weight * battery->positiveMaterialsRatio * battery->mn *
            battery->compoundRatio * battery->mn_recycleRatio * metalPriceMap.value("Mn");
    double ni_quotation = weight * battery->positiveMaterialsRatio * battery->ni *
            battery->compoundRatio * battery->ni_recycleRatio * metalPriceMap.value("Ni");

    double cu_quotation = weight * battery->cu * battery->cu_recycleRatio * metalPriceMap.value("Cu");

    qDebug()<<li_quotation<<co_quotation<<mn_quotation<<ni_quotation<<cu_quotation;


    //final price
    double finalPrice = (li_quotation+co_quotation+mn_quotation+ni_quotation+cu_quotation)
            - battery->otherCost - price_per_kilo * weight;

    return finalPrice >0? finalPrice*0.15:0;

}
