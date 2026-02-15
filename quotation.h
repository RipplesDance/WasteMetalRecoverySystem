#ifndef QUOTATION_H
#define QUOTATION_H

#include<QDebug>
#include<QMap>
#include<QMessageBox>
#include<QDataStream>
#include"batteryMaterialConcentration.h"

class quotation
{

public:
    quotation();
    ~quotation();
    double quotationCaculator(QString type, double energyDensity, double weight, double SOH, QMap<QString, double> metalPriceMap);
    void init();
    void setProperty(double unitPrice_80, double unitPrice_90,
                     double price_per_kilo, double profit);

    friend QDataStream &operator<<(QDataStream &out, const quotation &data);
    friend QDataStream &operator>>(QDataStream &in, quotation &data);

private:
    QMap<QString, batteryMaterialConcentration*> batteryMap;

public:

    //coefficient
    double Li_to_LCE;
    double Ni_to_NiSo4; // 6H₂O  NiSO₄·6H₂O
    double Co_to_CoSo4; // 7H₂O  CoSO₄·7H₂O
    double Mn_to_MnSo4;// 1H₂O MnSO₄·H₂O
    double transitionRatio; //cost, efficiency, etc..

    //property
    double unitPrice_80;//CNY/Wh. Depends on market
    double unitPrice_90;//CNY/Wh. Depends on market

    double price_per_kilo; //include electricity fee， labor fee, and chemicals fee. Count in CNY.
    double profit;
};

#endif // QUOTATION_H
