#include "quotation.h"

quotation::quotation()
{
    init();

    batteryMaterialConcentration* LCO = new batteryMaterialConcentration(0.071,0.602,0,0,0.1, 0.93, 0.35);
    LCO->setRecycleRatio(0.85, 0.95,0,0,0.98,0.95);
    batteryMap.insert("钴酸锂电池", LCO);

    setProperty(0.3, 0.35,45,0.2);
}

quotation:: ~quotation()
{
}

void quotation::init()
{

   Li_to_LCE = 73.89/ (6.94*2); // Li₂CO₃ = 73.89, Li = 6.94
   Ni_to_NiSo4 = 262.87/ 58.69; // 6H₂O  NiSO₄·6H₂O = 262.87, Ni = 58.69
   Co_to_CoSo4 = 281.10/ 58.93; // 7H₂O  CoSO₄·7H₂O = 281.10, Co = 58.93
   Mn_to_MnSo4 = 169.02/ 54.94;// 1H₂O MnSO₄·H₂O = 169.02, Mn = 54.94

   transitionRatio = 0.8;
}

void quotation::setProperty(double unitPrice_80, double unitPrice_90,
                                               double price_per_kilo, double profit)
{
    this->unitPrice_80 = unitPrice_80;
    this->unitPrice_90 = unitPrice_90;

    this->price_per_kilo = price_per_kilo;
    this->profit = profit;
}

double quotation::quotationCaculator(QString type, double energyDensity, double weight, double SOH,
                                     QMap<QString, double> metalPriceMap)
{
    if(!batteryMap.contains(type))
    {
        QMessageBox::information(nullptr,"提示", "当前暂不支持该电池的报价");
        return 0.0;
    }

    batteryMaterialConcentration* battery = batteryMap.value(type);

    if(SOH >= 0.8 && energyDensity > 0)
    {
        //reusable
        double finalPrice = weight * energyDensity * SOH;
        if(SOH>=0.9)
            return finalPrice * unitPrice_90;
        return finalPrice * unitPrice_80;
    }

    double positiveMaterial = weight * battery->positiveMaterialsRatio * battery->positiveMaterial_recycleRatio;
    double positiveMaterialCompound = positiveMaterial * battery->compoundRatio;

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

    //final price
    double finalPrice = (li_quotation+co_quotation+mn_quotation+ni_quotation+cu_quotation)
            - price_per_kilo * weight;

    return finalPrice > 0 ? finalPrice* (1 - profit) : 0;

}

QDataStream &operator<<(QDataStream &out, const quotation &data)
{
    out<<data.transitionRatio << data.unitPrice_80 << data.unitPrice_90 << data.price_per_kilo << data.profit;
    return out;
}

QDataStream &operator>>(QDataStream &in, quotation &data)
{
    in>>data.transitionRatio >> data.unitPrice_80 >> data.unitPrice_90 >> data.price_per_kilo >> data.profit;
    return in;
}
