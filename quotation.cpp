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
   price_per_kilo = 45;
   profit = 0.2;

   Li_to_LCE = 73.89/ (6.94*2); // Li₂CO₃ = 73.89, Li = 6.94
   Ni_to_NiSo4 = 262.87/ 58.69; // 6H₂O  NiSO₄·6H₂O = 262.87, Ni = 58.69
   Co_to_CoSo4 = 281.10/ 58.93; // 7H₂O  CoSO₄·7H₂O = 281.10, Co = 58.93
   Mn_to_MnSo4 = 169.02/ 54.94;// 1H₂O MnSO₄·H₂O = 169.02, Mn = 54.94

   qDebug()<<Li_to_LCE<<Ni_to_NiSo4;

   transitionRatio = 0.8;
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

    //metal amount
    double li_amount =  positiveMaterialCompound * battery->li * battery->li_recycleRatio;
    double co_amount = positiveMaterialCompound * battery->co * battery->co_recycleRatio;
    double mn_amount = positiveMaterialCompound * battery->mn * battery->mn_recycleRatio;
    double ni_amount = positiveMaterialCompound * battery->ni * battery->ni_recycleRatio;

    //metal price
    double li_quotation = li_amount * Li_to_LCE * metalPriceMap.value("Li") * transitionRatio;
    double ni_quotation = ni_amount * Ni_to_NiSo4 * metalPriceMap.value("Ni") * transitionRatio;
    double co_quotation = co_amount * Co_to_CoSo4 * metalPriceMap.value("Co") * transitionRatio;
    double mn_quotation = mn_amount * Mn_to_MnSo4 * metalPriceMap.value("Mn") * transitionRatio;

    double cu_quotation = weight * battery->cu * battery->cu_recycleRatio * metalPriceMap.value("Cu");

    qDebug()<<li_quotation << co_quotation<<mn_quotation<<ni_quotation<<cu_quotation;

    //final price
    double finalPrice = (li_quotation+co_quotation+mn_quotation+ni_quotation+cu_quotation)
            - battery->otherCost - price_per_kilo * weight;

    return finalPrice > 0 ? finalPrice* (1 - profit) : 0;

}
