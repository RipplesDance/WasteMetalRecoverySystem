#ifndef QUOTATION_H
#define QUOTATION_H

#include<QDebug>
#include<QMap>
#include<QMessageBox>
#include"batteryMaterialConcentration.h"

class quotation
{

public:
    quotation();
    ~quotation();
    double quotationCaculator(QString type, double energyDensity, double weight, double SOH, QMap<QString, double> metalPriceMap);
    void init();

private:
    QMap<QString, batteryMaterialConcentration*> batteryMap;

public:

    //coefficient
    double Li_to_LCE;
    double Ni_to_NiSo4; // 6H₂O  NiSO₄·6H₂O
    double Co_to_CoSo4; // 7H₂O  CoSO₄·7H₂O
    double Mn_to_MnSo4;// 1H₂O MnSO₄·H₂O
    double transitionRatio; //cost, efficiency, etc..
};

#endif // QUOTATION_H
